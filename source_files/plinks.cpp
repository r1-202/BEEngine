#include "plinks.h"
using namespace BEPhysics;

real ParticleLink::currentLength() const
{
  Vector3 relative_position = particle[0]->position - particle[1]->position;
  return glm::length(relative_position);
}

unsigned int ParticleCable::addContact(ParticleContact* contact, unsigned int limit) const
{
  real length = currentLength();
  if (length < max_length)
    return 0;

  contact->particle[0] = particle[0];
  contact->particle[1] = particle[1];


  Vector3 normal = particle[1]->position - particle[0]->position;
  normal = glm::normalize(normal);

  contact->contact_normal = normal;
  contact->penetration = length - max_length;
  contact->restitution = restitution;

  return 1;
}

unsigned int ParticleRod::addContact(ParticleContact* contact, unsigned int limit) const
{
  real current_length = currentLength();
  if (current_length == length)
    return 0;

  contact->particle[0] = particle[0];
  contact->particle[1] = particle[1];

  Vector3 normal = particle[1]->position - particle[0]->position;
  normal = glm::normalize(normal);

  if (current_length > length)
  {
    contact->contact_normal = normal;
    contact->penetration = current_length - length;
  }
  else
  {
    contact->contact_normal = -normal;
    contact->penetration = length - current_length;
  }

  contact->restitution = 0;

  return 1;
}

real ParticleConstraint::currentLength() const
{
  Vector3 relative_position = particle->position - anchor;
  return glm::length(relative_position);
}

unsigned ParticleCableConstraint::addContact(ParticleContact* contact, unsigned int limit) const
{
  real length = currentLength();

  if (length <= max_length)
    return 0;

  contact->particle[0] = particle;
  contact->particle[1] = NULL;

  Vector3 normal = anchor - particle->position;
  normal = glm::normalize(normal);
  contact->contact_normal = normal;

  contact->penetration = length - max_length;
  contact->restitution = restitution;

  return 1;
}

unsigned int ParticleRodConstraint::addContact(ParticleContact* contact, unsigned int limit) const
{
  real current_length = currentLength();
  if (current_length == length)
    return 0;

  contact->particle[0] = particle;
  contact->particle[1] = 0;

  Vector3 normal = anchor - particle->position;
  normal = glm::normalize(normal);

  if (current_length > length)
  {
    contact->contact_normal = normal;
    contact->penetration = current_length - length;
  }
  else
  {
    contact->contact_normal = -normal;
    contact->penetration = length - current_length;
  }

  contact->restitution = 0;

  return 1;

}