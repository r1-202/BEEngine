#pragma once
#include "particle.h"
namespace BEPhysics
{
  class ParticleForceGenerator
  {

  public:

    virtual void applyForce(Particle* particle, real delta_time) = 0;
  };

  class ParticleGravity : public ParticleForceGenerator
  {

    Vector3 gravity;

  public:

    ParticleGravity(const Vector3& gravity);

    virtual void applyForce(Particle* particle, real delta_time);
  };

  class ParticleDrag : public ParticleForceGenerator
  {

    real k1;
    real k2;

  public:

    ParticleDrag(real k1, real k2);

    virtual void applyForce(Particle* particle, real delta_time);
  };

  class ParticleAnchoredSpring : public ParticleForceGenerator
  {
  protected:

    Vector3* anchor;

    real k;

    real l;

  public:
    ParticleAnchoredSpring();

    ParticleAnchoredSpring(Vector3* anchor, real k, real l);

    const Vector3* getAnchor() const { return anchor; }

    void init(Vector3* anchor, real k, real l);

    virtual void applyForce(Particle* particle, real delta_time);
  };


  class ParticleAnchoredBungee : public ParticleAnchoredSpring
  {
  public:

    virtual void applyForce(Particle* particle, real delta_time);
  };


  class ParticleFakeSpring : public ParticleForceGenerator
  {

    Vector3* anchor;

    real k;

    real damping;

  public:


    ParticleFakeSpring(Vector3* anchor, real k, real damping);

    virtual void applyForce(Particle* particle, real delta_time);
  };


  class ParticleSpring : public ParticleForceGenerator
  {

    Particle* other;

    real k;

    real l;

  public:

    ParticleSpring(Particle* other, real k, real l);

    virtual void applyForce(Particle* particle, real delta_time);
  };


  class ParticleBungee : public ParticleForceGenerator
  {

    Particle* other;

    real k;

    real l;

  public:


    ParticleBungee(Particle* other, real k, real l);

    virtual void applyForce(Particle* particle, real delta_time);
  };


  class ParticleBuoyancy : public ParticleForceGenerator
  {

    real max_depth;


    real volume;


    real water_height;


    real liquid_density;

  public:

    ParticleBuoyancy(real max_depth, real volume, real water_height, real liquid_density = 1000.0f);

    virtual void applyForce(Particle* particle, real duration);
  };

  class ParticleForceRegistration
  {
  public:

    Particle* particle;
    ParticleForceGenerator* fg;

    ParticleForceRegistration(Particle* particle, ParticleForceGenerator* fg);
  };
}