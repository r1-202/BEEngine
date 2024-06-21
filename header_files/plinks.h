#pragma once
#include "pcontacts.h"
namespace BEPhysics
{
  class ParticleLink: public ParticleContactGenerator
  {
  public:
    Particle* particle[2];
  protected:
    real currentLength() const;
  public:
    virtual unsigned int addContact(ParticleContact* contact, unsigned int limit) const = 0;
  };

  class ParticleCable : public ParticleLink
  {
  public:
    real max_length;
    real restitution;
  public:
    virtual unsigned int addContact(ParticleContact* contact, unsigned int limit) const;
  };

  class ParticleRod : public ParticleLink
  {
  public:
    real length;
  public:
    virtual unsigned int addContact(ParticleContact* contact, unsigned int limit) const;
  };

  class ParticleConstraint : public ParticleContactGenerator
  {
  public:
    Particle* particle;
    Vector3 anchor;
  protected:
    real currentLength() const;
  public:
    virtual unsigned int addContact(ParticleContact* contact, unsigned int limit) const = 0;
  };

  class ParticleCableConstraint: public ParticleConstraint
  {
  public:
    real max_length;
    real restitution;
  public:
    virtual unsigned int addContact(ParticleContact* contact, unsigned int limit) const;
  };

  class ParticleRodConstraint : public ParticleConstraint
  {
  public:
    real length;
  public:
    virtual unsigned int addContact(ParticleContact* contact, unsigned int limit) const;
  };
};
