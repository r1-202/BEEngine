#include "fgen.h"
#include <iostream>
using namespace BEPhysics;

Buoyancy::Buoyancy(const Vector3& cOfB, real max_depth, real volume,
  real water_height, real liquid_density )
{
  centre_of_buoyancy = cOfB;
  this->liquid_density = liquid_density;
  this->max_depth = max_depth;
  this->volume = volume;
  this->water_height = water_height;
}

void Buoyancy::updateForce(RigidBody* body, real delta_time)
{

  Vector3 point_in_world = body->getPointInWorldSpace(centre_of_buoyancy);
  real depth = point_in_world.y;

  if (depth >= water_height + max_depth) return;
  Vector3 force(0, 0, 0);

  if (depth <= water_height - max_depth)
  {
    force.y = liquid_density * volume;
    body->addForceAtBodyPoint(force, centre_of_buoyancy);
    return;
  }
  force.y = liquid_density * volume * (water_height - (depth - max_depth)) / (2 * max_depth);
  body->addForceAtBodyPoint(force, centre_of_buoyancy);
}

Gravity::Gravity(const Vector3& gravity) : gravity(gravity) {}

void Gravity::updateForce(RigidBody* body, real delta_time)
{
  if (!body->hasFiniteMass())
    return;
  body->addForce(body->getMass() * gravity);
}

Spring::Spring(const Vector3& local_connection_point,
  RigidBody* other,
  const Vector3& other_connection_point,
  real k, real l) : connection_point(local_connection_point),
  other(other),
  other_connection_point(other_connection_point),
  k(k), l(l) {}

void Spring::updateForce(RigidBody* body, real delta_time)
{
  Vector3 l_ws = body->getPointInWorldSpace(connection_point);
  Vector3 o_ws = other->getPointInWorldSpace(other_connection_point);

  Vector3 force = l_ws - o_ws;

  real magnitude = glm::length(force);
  magnitude = magnitude - l;
  magnitude *= -k;

  force = glm::normalize(force);
  force *= magnitude;
  body->addForceAtPoint(force, l_ws);
}

Aero::Aero(const Matrix3& tensor, const Vector3& position, const Vector3* windspeed)
{
  Aero::tensor = tensor;
  Aero::position = position;
  Aero::windspeed = windspeed;
}

void Aero::updateForce(RigidBody* body, real delta_time)
{
  updateForceFromTensor(body, delta_time, tensor);
}

void Aero::updateForceFromTensor(RigidBody* body, real delta_time, const Matrix3& tensor)
{
  Vector3 velocity = body->velocity;
  velocity += *windspeed;
  Vector3 body_velocity = body->getDirectionInLocalSpace(velocity);
  Vector3 body_force = tensor * body_velocity;
  Vector3 force = body->getDirectionInWorldSpace(body_force);
  body->addForceAtBodyPoint(force, position);
}

AeroControl::AeroControl(const Matrix3& base, const Matrix3& min, const Matrix3& max,
  const Vector3& position, const Vector3* windspeed) : Aero(base, position, windspeed)
{
  min_tensor = min;
  max_tensor = max;
  control_setting = 0.;
}

Matrix3 AeroControl::getTensor()
{
  if (control_setting <= -1.0)
    return min_tensor;
  else if (control_setting >= 1.0)
    return max_tensor;
  else if (control_setting < 0)
  {
    real t = 1 + control_setting;
    return (-control_setting) * min_tensor + (1 + control_setting) * tensor;
  }
  else if (control_setting > 0)
    return (1 - control_setting) * tensor + control_setting * max_tensor;
  else
    return tensor;
}

void AeroControl::setControl(real value)
{
  control_setting = value;
}

void AeroControl::updateForce(RigidBody* body, real delta_time)
{
  updateForceFromTensor(body, delta_time, getTensor());
}

void Explosion::updateForce(RigidBody* body, real delta_time)
{

}

ForceRegistration::ForceRegistration(RigidBody* body, ForceGenerator* fg)
{
  this->body = body;
  this->fg = fg;
}