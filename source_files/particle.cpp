#include "particle.h"
using namespace BEPhysics;
void Particle::integrate(real delta_time)
{
  if (inverse_mass <= 0.)
    return;

  position += delta_time * velocity;

  Vector3 resulting_acceleration = acceleration;
  resulting_acceleration += inverse_mass * force_accumulator;

  velocity += delta_time * resulting_acceleration;

  velocity *= pow(damping, delta_time);

  force_accumulator = Vector3(0.);
}

bool Particle::hasFiniteMass()
{
  return inverse_mass > 0.;
}

real Particle::getMass()
{
  return 1. / inverse_mass;
}