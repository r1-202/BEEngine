#include "body.h"
#include "quatExtention.h"
#include "physicsParameters.h"
using namespace BEPhysics;
static inline void _transformInertiaTensor(Matrix3& iit_world, const Quaternion& q, const Matrix3& iit_body, const Matrix4& rotmat)
{
  Matrix3 mat = Matrix3(rotmat);
  iit_world = mat * iit_body * glm::inverse(mat);
}

static inline void _calculateTransformMatrix(Matrix4& transform_matrix, const Vector3& position, const Quaternion& orientation)
{
  transform_matrix = glm::toMat4(orientation);
  transform_matrix[3] = Vector4(position, 1);
}

void RigidBody::calculateDerivedData()
{
  orientation = glm::normalize(orientation);
  _calculateTransformMatrix(transform_matrix, position, orientation);
  _transformInertiaTensor(inverse_inertia_tensor_world, orientation, inverse_inertia_tensor, transform_matrix);
}

void RigidBody::integrate(real delta_time)
{
  if (!is_awake)
    return;

  last_frame_acceleration = acceleration;
  last_frame_acceleration += inverse_mass * force_accumulator;
  Vector3 angular_acceleration = inverse_inertia_tensor_world * torque_accumulator;
  velocity += delta_time * last_frame_acceleration;
  angular_velocity += delta_time * angular_acceleration;
  velocity *= real_pow(linear_damping, delta_time);
  angular_velocity *= real_pow(angular_damping, delta_time);
  position += delta_time * velocity;
  orientation = addVectorToQuat(orientation, delta_time * angular_velocity);
  calculateDerivedData();
  clearAccumulators();
  if (can_sleep)
  {
    real current_motion = glm::dot(velocity, velocity) + glm::dot(angular_velocity, angular_velocity);
    real bias = real_pow(0.5, delta_time);
    motion = bias * motion + (1 - bias) * current_motion;
    if (motion < sleep_epsilon)
      setAwake(false);
    else if (motion > 10 * sleep_epsilon)
      motion = 10 * sleep_epsilon;
  }
}

real RigidBody::getMass() const
{
  if (inverse_mass == 0)
    return REAL_MAX;
  else
    return (real)1. / inverse_mass;
}

bool RigidBody::hasFiniteMass() const
{
  return inverse_mass > 0.0f;
}

Vector3 RigidBody::getPointInLocalSpace(const Vector3& point) const
{
  return glm::inverse(transform_matrix) * Vector4(point, 1);
}

Vector3 RigidBody::getPointInWorldSpace(const Vector3& point) const
{
  return transform_matrix * Vector4(point, 1);
}

Vector3 RigidBody::getDirectionInLocalSpace(const Vector3& direction) const
{
  return glm::transpose(Matrix3(transform_matrix)) * direction;
}

Vector3 RigidBody::getDirectionInWorldSpace(const Vector3& direction) const
{
  return glm::inverse(glm::transpose(Matrix3(transform_matrix))) * direction;
}

void RigidBody::setAwake(const bool awake)
{
  if (awake)
  {
    is_awake = true;
    motion = sleep_epsilon * 2.;
  }
  else
  {
    is_awake = false;
    velocity = Vector3(0.);
    angular_velocity = Vector3(0.);
  }
}

void RigidBody::setCanSleep(const bool can_sleep)
{
  this->can_sleep = can_sleep;
  if (!can_sleep && !is_awake)
    setAwake();
}

void RigidBody::clearAccumulators()
{
  force_accumulator = Vector3(0.);
  torque_accumulator = Vector3(0.);
}

void RigidBody::addForce(const Vector3& force)
{
  force_accumulator += force;
  is_awake = true;
}

void RigidBody::addForceAtBodyPoint(const Vector3& force, const Vector3& point)
{
  Vector3 pt = getPointInWorldSpace(point);
  addForceAtPoint(force, pt);
}

void RigidBody::addForceAtPoint(const Vector3& force, const Vector3& point)
{
  Vector3 pt = point;
  pt -= position;

  force_accumulator += force;
  torque_accumulator += glm::cross(pt, force);
  is_awake = true;
}

void RigidBody::addTorque(const Vector3& torque)
{
  torque_accumulator += torque;
  is_awake = true;
}