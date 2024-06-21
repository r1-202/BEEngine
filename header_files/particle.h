#pragma once
#include <glad/glad.h>
#include "precision.h"
namespace BEPhysics
{
  class Particle
  {
  public:
    real inverse_mass;
    real damping;
    Vector3 position;
    Vector3 velocity;
    Vector3 acceleration;
    Vector3 force_accumulator;

    void integrate(real delta_time);
    bool hasFiniteMass();
    real getMass();
  };
};

