#include "pfgen.h"
#include <iostream>
using namespace BEPhysics;

ParticleGravity::ParticleGravity(const Vector3& gravity) : gravity(gravity) {}

void ParticleGravity::applyForce(Particle* particle, real delta_time)
{
  if (!particle->hasFiniteMass())
    return;
  particle->force_accumulator += particle->getMass() * gravity;
}

ParticleDrag::ParticleDrag(real k1, real k2) : k1(k1), k2(k2) {}

void ParticleDrag::applyForce(Particle* particle, real delta_time)
{
  Vector3 force;

  force = particle->velocity;
  real drag_coefficient = glm::length(force);
  drag_coefficient = k1 * drag_coefficient + k2 * drag_coefficient * drag_coefficient;

  force = glm::normalize(force);
  force *= -drag_coefficient;
  particle->force_accumulator += force;
}

ParticleSpring::ParticleSpring(Particle* other, real k, real l) : other(other), k(k), l(l) {}

void ParticleSpring::applyForce(Particle* particle, real delta_time)
{
  Vector3 force;
  force = particle->position;
  force -= other->position;

  real magnitude = glm::length(force);
  magnitude = -k*(magnitude - l);

  force = glm::normalize(force);
  force *= magnitude;
  particle->force_accumulator += force;
}

ParticleBuoyancy::ParticleBuoyancy(real max_depth, real volume, real water_height, real liquid_density) :
  max_depth(max_depth), volume(volume), water_height(water_height), liquid_density(liquid_density) {}

void ParticleBuoyancy::applyForce(Particle* particle, real delta_time)
{
  real depth = particle->position.y;

  if (depth - max_depth >= water_height)
    return;

  Vector3 force(0, 0, 0);

  if (depth + max_depth <= water_height)
  {
    force.y = liquid_density * volume;
    particle->force_accumulator += force;
    return;
  }

  force.y = liquid_density * volume * (water_height - (depth - max_depth)) / (2 * max_depth);

  particle->force_accumulator += force;
}

ParticleBungee::ParticleBungee(Particle* other, real k, real l) : other(other), k(k), l(l) {}

void ParticleBungee::applyForce(Particle* particle, real delta_time)
{
  Vector3 force;
  force = particle->position;
  force -= other->position;

  real magnitude = glm::length(force);
  if (magnitude <= l)
    return;
  magnitude = -k * (magnitude - l);

  force = glm::normalize(force);
  force *= magnitude;
  particle->force_accumulator += force;
}

ParticleFakeSpring::ParticleFakeSpring(Vector3* anchor, real k, real damping):
  anchor(anchor), k(k), damping(damping) {}

void ParticleFakeSpring::applyForce(Particle* particle, real delta_time)
{
  //solve d.e for required position, get required acceleration to get it there (from speed and position)
  //apply force needed to get that acceleration
  //there are some mistakes in this code
  if (!particle->hasFiniteMass())
    return;

  Vector3 position;
  position = particle->position;
  position -= *anchor;

  real gamma = 0.5 * sqrt(4 * k - damping * damping);

  if (gamma == 0.)
    return;

  Vector3 c = position * (real)(damping / (2.0 * gamma)) + particle->velocity * (real)(1. / gamma);

  Vector3 target = position * real_cos(gamma * delta_time) + c * real_sin(gamma * delta_time);
  target *= real_exp(-0.5 * delta_time * damping);

  Vector3 acceleration = (target - position) * (real)(1.0 / (delta_time * delta_time)) - particle->velocity * (real)(1. / delta_time);

  particle->force_accumulator += acceleration * particle->getMass();
}


ParticleAnchoredSpring::ParticleAnchoredSpring() {}

ParticleAnchoredSpring::ParticleAnchoredSpring(Vector3* anchor, real k, real l) :
  anchor(anchor), k(k), l(l) {}

void ParticleAnchoredSpring::init(Vector3* anchor, real k, real l)
{
  this->anchor = anchor;
  this->k = k;
  this->l = l;
}

void ParticleAnchoredBungee::applyForce(Particle* particle, real delta_time)
{
  Vector3 force;
  force = particle->position;
  force -= *anchor;

  real magnitude = glm::length(force);
  if (magnitude <= l)
    return;

  magnitude = -k * (magnitude - l);

  force = glm::normalize(force);
  force *= magnitude;
  particle->force_accumulator += force;
}

void ParticleAnchoredSpring::applyForce(Particle* particle, real delta_time)
{
  Vector3 force;
  force = particle->position;
  force -= *anchor;

  real magnitude = glm::length(force);
  magnitude = -k * (magnitude - l);
  std::cout << magnitude << std::endl;
  force = glm::normalize(force);
  force *= magnitude;
  particle->force_accumulator += force;
}

ParticleForceRegistration::ParticleForceRegistration(Particle* particle, ParticleForceGenerator* fg)
{
  this->particle = particle;
  this->fg = fg;
}