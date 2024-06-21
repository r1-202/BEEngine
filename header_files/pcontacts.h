#pragma once
#include "particle.h"
namespace BEPhysics
{
  class ParticleContact
  {
  public:
    Particle* particle[2];
    real restitution;
    Vector3 contact_normal;
    real penetration;
    Vector3 particle_movement[2];

    void resolve(real delta_time);
    real calculateSeparatingVelocity() const;

    void resolveVelocity(real delta_time);
    void resolvePenetration(real delta_time);
  };

  class ParticleContactResolver
  {
  public:
    unsigned int iterations;
    unsigned int iterations_used;
  public:
    ParticleContactResolver() {}
    ParticleContactResolver(unsigned int iterations);
    void setIterations(unsigned int iterations);
    void resolveContacts(ParticleContact* contact, unsigned int nr_contacts, real delta_time);
  };

  class ParticleContactGenerator
  {
  public:
    virtual unsigned int addContact(ParticleContact* contact, unsigned int limit) const = 0;
  };
};

