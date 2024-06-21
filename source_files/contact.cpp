#include "contact.h"
#include "quatExtention.h"
using namespace BEPhysics;
void Contact::setBodyData(RigidBody* one, RigidBody* two, real friction, real restitution)
{
  body[0] = one;
  body[1] = two;
  this->friction = friction;
  this->restitution = restitution;
}

void Contact::matchAwakeState()
{
  if (!body[1])
    return;
  bool body_0_awake = body[0]->is_awake;
  bool body_1_awake = body[1]->is_awake;
  //^ = XOR
  if (body_0_awake ^ body_1_awake)
  {
    if (body_0_awake)
      body[1]->setAwake();
    else
      body[0]->setAwake();
  }
}

void Contact::swapBodies()
{
  contact_normal *= -1;
  RigidBody* temp = body[0];
  body[0] = body[1];
  body[1] = temp;
}

void Contact::calculateContactBasis()
{
  Vector3 contact_tangent[2];
  if (real_abs(contact_normal.x) > real_abs(contact_normal.y))
  {
    contact_tangent[0] = glm::normalize(glm::cross(Vector3(0, 1, 0), contact_normal));
    contact_tangent[1] = glm::cross(contact_normal, contact_tangent[0]);
  }
  else
  {
    contact_tangent[0] = glm::normalize(glm::cross(Vector3(1, 0, 0), contact_normal));
    contact_tangent[1] = glm::cross(contact_normal, contact_tangent[0]);
  }
  contact_to_world = Matrix3(contact_normal, contact_tangent[0], contact_tangent[1]);
}

Vector3 Contact::calculateLocalVelocity(unsigned int body_index, real delta_time)
{
  RigidBody* this_body = body[body_index];
  Vector3 velocity = glm::cross(this_body->angular_velocity, relative_contact_position[body_index]);
  velocity += this_body->velocity;
  Vector3 contact_velocity = glm::transpose(contact_to_world) * velocity;
  Vector3 acc_velocity = this_body->last_frame_acceleration * delta_time;
  acc_velocity = glm::transpose(contact_to_world) * acc_velocity;
  acc_velocity.x = 0;
  contact_velocity += acc_velocity;
  return contact_velocity;
}

void Contact::calculateDesiredDeltaVelocity(real delta_time)
{
  const static real velocity_limit = (real)0.25f;
  real velocity_from_acc = 0;
  if (body[0]->is_awake)
  {
    velocity_from_acc += glm::dot(body[0]->last_frame_acceleration * delta_time, contact_normal);
  }
  if (body[1] && body[1]->is_awake)
  {
    velocity_from_acc -= glm::dot(body[1]->last_frame_acceleration * delta_time, contact_normal);
  }
  real this_restitution = restitution;
  if (real_abs(contact_velocity.x) < velocity_limit)
  {
    this_restitution = (real)0.0f;
  }
  desired_delta_velocity = -contact_velocity.x - this_restitution * (contact_velocity.x - velocity_from_acc);
}

void Contact::calculateInternals(real delta_time)
{
  if (!body[0])
    swapBodies();

  calculateContactBasis();
  relative_contact_position[0] = contact_point - body[0]->position;
  if (body[1])
    relative_contact_position[1] = contact_point - body[1]->position;

  contact_velocity = calculateLocalVelocity(0, delta_time);
  if (body[1])
    contact_velocity -= calculateLocalVelocity(1, delta_time);

  calculateDesiredDeltaVelocity(delta_time);

}

void Contact::applyVelocityChange(Vector3 velocity_change[2], Vector3 angular_velocity_change[2])
{
  Matrix3 inverse_inertia_tensor[2];
  inverse_inertia_tensor[0] = body[0]->inverse_inertia_tensor_world;
  if (body[1])
    inverse_inertia_tensor[1] = body[1]->inverse_inertia_tensor_world;

  Vector3 impulse_contact;
  if (friction == (real)0.)
    impulse_contact = calculateFrictionlessImpulse(inverse_inertia_tensor);
  else
    impulse_contact = calculateFrictionImpulse(inverse_inertia_tensor);

  Vector3 impulse = contact_to_world * impulse_contact;

  Vector3 impulsive_torque = glm::cross(relative_contact_position[0], impulse);
  angular_velocity_change[0] = inverse_inertia_tensor[0] * impulsive_torque;
  velocity_change[0] = Vector3(0);
  velocity_change[0] = body[0]->inverse_mass * impulse;

  body[0]->velocity += velocity_change[0];
  body[0]->angular_velocity += angular_velocity_change[0];
  if (body[1])
  {
    Vector3 impulsive_torque = glm::cross(relative_contact_position[1], -impulse);
    angular_velocity_change[1] = inverse_inertia_tensor[1] * impulsive_torque;
    velocity_change[1] = Vector3(0);
    velocity_change[1] = body[1]->inverse_mass * (-impulse);

    body[1]->velocity += velocity_change[1];
    body[1]->angular_velocity += angular_velocity_change[1];
  }
}

Vector3 Contact::calculateFrictionlessImpulse(Matrix3* inverse_inertia_tensor)
{
  Vector3 impulse_contact;
  //unit impulse in contact_normal direction
  Vector3 delta_vel_world = glm::cross(relative_contact_position[0], contact_normal);
  delta_vel_world = inverse_inertia_tensor[0] * delta_vel_world;
  delta_vel_world = glm::cross(delta_vel_world, relative_contact_position[0]);

  real delta_velocity = glm::dot(delta_vel_world, contact_normal);
  delta_velocity += body[0]->inverse_mass;

  if (body[1])
  {
    Vector3 delta_vel_world = glm::cross(relative_contact_position[1], -contact_normal);
    delta_vel_world = inverse_inertia_tensor[1] * delta_vel_world;
    delta_vel_world = glm::cross(delta_vel_world, relative_contact_position[1]);

    delta_velocity -= glm::dot(delta_vel_world, contact_normal);
    delta_velocity += body[1]->inverse_mass;
  }

  impulse_contact.x = desired_delta_velocity / delta_velocity;
  impulse_contact.y = 0;
  impulse_contact.z = 0;
  return impulse_contact;
}

Vector3 Contact::calculateFrictionImpulse(Matrix3* inverse_inertia_tensor)
{
  Vector3 impulse_contact;
  real inverse_mass = body[0]->inverse_mass;
  Matrix3 impulse_to_torque = glm::matrixCross3(relative_contact_position[0]);
  Matrix3 delta_vel_world = impulse_to_torque;
  delta_vel_world *= inverse_inertia_tensor[0];
  delta_vel_world *= impulse_to_torque;
  delta_vel_world *= -1;
  if (body[1])
  {
    impulse_to_torque = glm::matrixCross3(relative_contact_position[1]);
    Matrix3 delta_vel_world2 = impulse_to_torque;
    delta_vel_world2 *= inverse_inertia_tensor[1];
    delta_vel_world2 *= impulse_to_torque;
    delta_vel_world2 *= -1;
    delta_vel_world += delta_vel_world2;
    inverse_mass += body[1]->inverse_mass;
  }

  Matrix3 delta_velocity = glm::transpose(contact_to_world);
  delta_velocity *= delta_vel_world;
  delta_velocity *= contact_to_world;

  delta_velocity[0][0] += inverse_mass;
  delta_velocity[1][1] += inverse_mass;
  delta_velocity[2][2] += inverse_mass;

  Matrix3 impulse_matrix = glm::inverse(delta_velocity);

  Vector3 vel_kill(desired_delta_velocity, -contact_velocity.y, -contact_velocity.z);

  impulse_contact = impulse_matrix * vel_kill;
  real planar_impulse = real_sqrt(impulse_contact.y * impulse_contact.y + impulse_contact.z * impulse_contact.z);
  if (planar_impulse > impulse_contact.x * friction)
  {
    impulse_contact.y /= planar_impulse;
    impulse_contact.z /= planar_impulse;

    impulse_contact.x = delta_velocity[0][0] +
      delta_velocity[1][0] * friction * impulse_contact.y +
      delta_velocity[2][0] * friction * impulse_contact.z;
    impulse_contact.x = desired_delta_velocity / impulse_contact.x;
    impulse_contact.y *= friction * impulse_contact.x;
    impulse_contact.z *= friction * impulse_contact.x;
  }
  return impulse_contact;
}

void Contact::applyPositionChange(Vector3 linear_change[2], Vector3 angular_change[2], real penetration)
{
  const real angular_limit = (real)0.2f;
  real angular_move[2];
  real linear_move[2];

  real total_i_inertia = 0;
  real linear_inertia[2];
  real angular_inertia[2];
  for (unsigned int i = 0; i < 2; i++) if (body[i])
  {
    Matrix3 inverse_inertia_tensor = body[i]->inverse_inertia_tensor_world;
    Vector3 angular_inertia_world = glm::cross(relative_contact_position[i], contact_normal);
    angular_inertia_world = inverse_inertia_tensor * angular_inertia_world;
    angular_inertia_world = glm::cross(angular_inertia_world, relative_contact_position[i]);
    angular_inertia[i] = glm::dot(angular_inertia_world, contact_normal);
    linear_inertia[i] = body[i]->inverse_mass;
    total_i_inertia += linear_inertia[i] + angular_inertia[i];
  }

  for (unsigned int i = 0; i < 2; i++) if (body[i])
  {
    real sign = (i == 0) ? 1 : -1;
    angular_move[i] = sign * penetration * (angular_inertia[i] / total_i_inertia);
    linear_move[i] = sign * penetration * (linear_inertia[i] / total_i_inertia);
    Vector3 projection = relative_contact_position[i];
    projection += (-glm::dot(relative_contact_position[i], contact_normal)) * contact_normal;
    real max_magnitude = angular_limit * glm::length(projection);
    if (angular_move[i] < -max_magnitude)
    {
      real total_move = angular_move[i] + linear_move[i];
      angular_move[i] = -max_magnitude;
      linear_move[i] = total_move - max_magnitude;
    }
    else if (angular_move[i] > max_magnitude)
    {
      real total_move = angular_move[i] + linear_move[i];
      angular_move[i] = max_magnitude;
      linear_move[i] = total_move - max_magnitude;
    }
    if (angular_move[i] == 0)
      angular_change[i] = Vector3(0);
    else
    {
      Vector3 target_angular_direction = glm::cross(relative_contact_position[i], contact_normal);
      Matrix3 inverse_inertia_tensor = body[i]->inverse_inertia_tensor_world;
      angular_change[i] = (angular_move[i] / angular_inertia[i]) * inverse_inertia_tensor * target_angular_direction;
    }
    linear_change[i] = contact_normal * linear_move[i];

    body[i]->position += linear_move[i] * contact_normal;
    body[i]->orientation = addVectorToQuat(body[i]->orientation, angular_change[i]);
    if (!body[i]->is_awake)
      body[i]->calculateDerivedData();
  }
}

ContactResolver::ContactResolver(unsigned int iterations, real velocity_epsilon, real position_epsilon)
{
  velocity_iterations = position_iterations = iterations;
  this->velocity_epsilon = velocity_epsilon;
  this->position_epsilon = position_epsilon;
}

ContactResolver::ContactResolver(unsigned int velocity_iterations, unsigned int position_iterations,
  real velocity_epsilon, real position_epsilon)
{
  this->velocity_iterations = velocity_iterations;
  this->position_iterations = position_iterations;
  this->velocity_epsilon = velocity_epsilon;
  this->position_epsilon = position_epsilon;
}

void ContactResolver::resolveContacts(Contact* contacts, unsigned int nr_contacts, real delta_time)
{
  if (nr_contacts == 0)
    return;
  if (!isValid())
    return;

  prepareContacts(contacts, nr_contacts, delta_time);
  adjustPositions(contacts, nr_contacts, delta_time);
  adjustVelocities(contacts, nr_contacts, delta_time);
}

void ContactResolver::prepareContacts(Contact* contacts, unsigned int nr_contacts, real delta_time)
{
  Contact* last_contact = contacts + nr_contacts;
  for (Contact* contact = contacts; contact < last_contact; ++contact)
  {
    contact->calculateInternals(delta_time);
  }
}

void ContactResolver::adjustVelocities(Contact* c, unsigned int nr_contacts, real delta_time)
{
  Vector3 velocity_change[2];
  Vector3 angular_velocity_change[2];
  Vector3 delta_vel;

  velocity_iterations_used = 0;
  while (velocity_iterations_used < velocity_iterations)
  {
    real max = velocity_epsilon;
    unsigned int index = nr_contacts;
    for (unsigned int i = 0; i < nr_contacts; i++)
    {
      if (c[i].desired_delta_velocity > max)
      {
        max = c[i].desired_delta_velocity;
        index = i;
      }
    }
    if (index == nr_contacts)
      break;
    c[index].matchAwakeState();
    c[index].applyVelocityChange(velocity_change, angular_velocity_change);
    for (unsigned int i = 0; i < nr_contacts; ++i)
    {
      for (unsigned int b = 0; b < 2; ++b) if (c[i].body[b])
      {
        for (unsigned int d = 0; d < 2; ++d)
        {
          if (c[i].body[b] = c[index].body[d])
          {
            delta_vel = velocity_change[d] + glm::cross(angular_velocity_change[d], c[i].relative_contact_position[b]);
            c[i].contact_velocity += (b ? -1 : 1) * glm::transpose(c[i].contact_to_world) * delta_vel;
            c[i].calculateDesiredDeltaVelocity(delta_time);
          }
        }
      }
    }
    ++velocity_iterations_used;
  }
}

void ContactResolver::adjustPositions(Contact* c, unsigned int nr_contacts, real delta_time)
{
  unsigned int i;
  unsigned int index;
  Vector3 linear_change[2];
  Vector3 angular_change[2];
  real max;
  Vector3 delta_position;

  position_iterations_used = 0;

  while (position_iterations_used < position_iterations)
  {
    max = position_epsilon;
    index = nr_contacts;
    for (i = 0; i < nr_contacts; i++)
    {
      if (c[i].penetration > max)
      {
        max = c[i].penetration;
        index = i;
      }
    }

    if (index == nr_contacts)
      break;
    c[index].matchAwakeState();
    c[index].applyPositionChange(linear_change, angular_change, max);
    for (i = 0; i < nr_contacts; i++)
    {
      for (unsigned int b = 0; b < 2; b++) if (c[i].body[b])
      {
        for (unsigned int d = 0; d < 2; d++)
        {
          if (c[i].body[b] == c[index].body[d])
          {
            delta_position = linear_change[d] + glm::cross(angular_change[d], c[i].relative_contact_position[b]);
            c[i].penetration = (b ? 1 : -1) * glm::dot(delta_position, c[i].contact_normal);
          }
        }
      }
    }
    position_iterations_used++;
  }
}