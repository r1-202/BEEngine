#include "quatExtention.h"
using namespace BEPhysics;
Quaternion BEPhysics::addVectorToQuat(Quaternion p, Vector3 v)
{
  Quaternion q(0, v.x, v.y, v.z);
  q *= p;
  p += (real)0.5 * q;
  return p;
}