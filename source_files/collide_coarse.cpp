#include "collide_coarse.h"
using namespace BEPhysics;
BoundingSphere::BoundingSphere(const Vector3& center, real radius)
{
  this->center = center;
  this->radius = radius;
}

BoundingSphere::BoundingSphere(const BoundingSphere& one, const BoundingSphere& two)
{
  Vector3 center_offset = two.center - one.center;
  real distance = glm::length2(center_offset);
  real radius_diff = two.radius - one.radius;

  if (radius_diff * radius_diff >= distance)
  {
    if (one.radius > two.radius)
    {
      center = one.center;
      radius = one.radius;
    }
    else
    {
      center = two.center;
      radius = two.radius;
    }
  }
  else
  {
    distance = real_sqrt(distance);
    radius = (distance + one.radius + two.radius) * ((real)0.5);
    center = one.center;
    if (distance > 0)
    {
      center += ((radius - one.radius)) * (center_offset / distance);
    }
  }
}

bool BoundingSphere::overlaps(const BoundingSphere& other) const
{
  real distance2 = glm::length2(center - other.center);
  return (distance2 < (radius + other.radius)* (radius + other.radius));
}

real BoundingSphere::getGrowth(const BoundingSphere& other) const
{
  BoundingSphere new_sphere(*this, other);
  return new_sphere.radius * new_sphere.radius - radius * radius;
}