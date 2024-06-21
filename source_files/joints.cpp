#include "joints.h"
using namespace BEPhysics;
unsigned Joint::addContact(Contact* contact, unsigned int limit) const
{
  Vector3 a_pos_world = body[0]->getPointInWorldSpace(position[0]);
  Vector3 b_pos_world = body[1]->getPointInWorldSpace(position[1]);
  Vector3 a_to_b = b_pos_world - a_pos_world;
  Vector3 normal = glm::normalize(a_to_b);
  real length = glm::length(a_to_b);
  if (length > error)
  {
    contact->setBodyData(body[0], body[1], 1.0, 0.);
    contact->contact_normal = normal;
    contact->contact_point = 0.5 * (a_pos_world + b_pos_world);
    contact->penetration = length - error;
    return 1;
  }
  return 0;
}

void Joint::set(RigidBody* a, const Vector3& a_pos, RigidBody* b, const Vector3& b_pos, real error)
{
  body[0] = a;
  body[1] = b;
  position[0] = a_pos;
  position[1] = b_pos;
  this->error = error;
}