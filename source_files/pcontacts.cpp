#include "pcontacts.h"
#include <float.h>
using namespace BEPhysics;
void ParticleContact::resolve(real delta_time)
{
  resolveVelocity(delta_time);
  resolvePenetration(delta_time);
}

real ParticleContact::calculateSeparatingVelocity() const
{
  Vector3 relative_velocity = particle[0]->velocity;
  if (particle[1])
    relative_velocity -= particle[1]->velocity;
  return glm::dot(relative_velocity, contact_normal);
}

void ParticleContact::resolveVelocity(real delta_time)
{
  real separating_velocity = calculateSeparatingVelocity();

  if (separating_velocity > 0)
    return;

  real new_separating_velocity = -separating_velocity * restitution;
  Vector3 acceleration_caused_velocity = particle[0]->acceleration;
  if (particle[1])
    acceleration_caused_velocity -= particle[1]->acceleration;
  real acceleration_caused_separating_velocity = delta_time * glm::dot(acceleration_caused_velocity, contact_normal);

  if (acceleration_caused_separating_velocity < 0)
  {
    new_separating_velocity += restitution * acceleration_caused_separating_velocity;

    if (new_separating_velocity < 0)
      new_separating_velocity = 0;
  }

  real delta_velocity = new_separating_velocity - separating_velocity;
  real total_inverse_mass = particle[0]->inverse_mass;
  if (particle[1])
    total_inverse_mass += particle[1]->inverse_mass;

  if (total_inverse_mass <= 0)
    return;

  real impulse = delta_velocity / total_inverse_mass;

  Vector3 impulse_per_inverse_mass = contact_normal * impulse;

  particle[0]->velocity = particle[0]->velocity + impulse_per_inverse_mass * particle[0]->inverse_mass;
  if (particle[1])
    particle[1]->velocity = particle[1]->velocity - impulse_per_inverse_mass * particle[1]->inverse_mass;
}

void ParticleContact::resolvePenetration(real delta_time)
{
  if (penetration <= 0)
    return;
  real total_inverse_mass = particle[0]->inverse_mass;
  if (particle[1])
    total_inverse_mass += particle[1]->inverse_mass;

  if (total_inverse_mass <= 0)
    return;

  Vector3 move_per_inverse_mass = (penetration / total_inverse_mass) * contact_normal;

  particle_movement[0] = move_per_inverse_mass * particle[0]->inverse_mass;
  if (particle[1])
    particle_movement[1] = move_per_inverse_mass * particle[0]->inverse_mass;
  else
    particle_movement[1] = Vector3(0);

  particle[0]->position = particle[0]->position + particle_movement[0];
  if (particle[1])
    particle[1]->position = particle[1]->position + particle_movement[1];
}

ParticleContactResolver::ParticleContactResolver(unsigned int iterations) : iterations(iterations) {}

void ParticleContactResolver::setIterations(unsigned int iterations)
{
  this->iterations = iterations;
}

void ParticleContactResolver::resolveContacts(ParticleContact* contact, unsigned int nr_contacts, real delta_time)
{
  unsigned int i;
  iterations_used = 0;
  while (iterations_used < iterations)
  {
    real max = DBL_MAX;
    unsigned int max_index = nr_contacts;
    for (i = 0; i < nr_contacts; i++)
    {
      real separating_velocity = contact[i].calculateSeparatingVelocity();
      if (separating_velocity < max && (separating_velocity < 0 || contact[i].penetration>0))
      {
        max = separating_velocity;
        max_index = i;
      }
    }
    if (max_index == nr_contacts)
      break;

    contact[max_index].resolve(delta_time);

    Vector3* move = contact[max_index].particle_movement;

    for (i = 0; i < nr_contacts; i++)
    {
      if (contact[i].particle[0] == contact[max_index].particle[0])
        contact[i].penetration -= glm::dot(move[0], contact[max_index].contact_normal);
      else if (contact[i].particle[0] == contact[max_index].particle[1])
          contact[i].penetration -= glm::dot(move[1], contact[max_index].contact_normal);
      if (contact[i].particle[1])
      {
        if (contact[i].particle[1] == contact[max_index].particle[0])
          contact[i].penetration += glm::dot(move[0], contact[max_index].contact_normal);
        else if(contact[i].particle[1]==contact[max_index].particle[1])
          contact[i].penetration += glm::dot(move[1], contact[max_index].contact_normal);
      }
    }
    iterations_used++;
  }
}
