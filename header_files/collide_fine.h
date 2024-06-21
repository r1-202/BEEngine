#pragma once
#include "contact.h"
namespace BEPhysics
{
  class IntersectionTests;
  class CollisionDetector;
  class CollisionPrimitive
  {
  public:
    friend class IntersectionTests;
    friend class CollisionDetector;

    RigidBody* body;
    Matrix4 offset;
    void calculateInternals();
    Vector3 getAxis(unsigned int index) const
    {
      return Vector3(offset[index].x, offset[index].y, offset[index].z);
    }
    const Matrix4& getTransform() const
    {
      return transform;
    }
  protected:
    Matrix4 transform;
  };

  class CollisionSphere : public CollisionPrimitive
  {
  public:
    real radius;
  };

  class CollisionPlane
  {
  public:
    Vector3 direction;
    real offset;
  };

  class CollisionBox : public CollisionPrimitive
  {
  public:
    Vector3 half_size;
  };

  class IntersectionTests
  {
  public:
    static bool sphereAndHalfSpace(const CollisionSphere& sphere, const CollisionPlane& plane);
    static bool sphereAndSphere(const CollisionSphere& one, const CollisionSphere& two);
    static bool boxAndBox(const CollisionBox& one, const CollisionBox& two);
    static bool boxAndHalfSpace(const CollisionBox& box, const CollisionPlane& plane);
  };

  struct CollisionData
  {
    Contact* contact_array;
    Contact* contacts;
    int contacts_left;
    unsigned int contact_count;
    real friction;
    real restitution;
    real tolerance;
    bool hasMoreContacts()
    {
      return contacts_left > 0;
    }
    void reset(unsigned int max_contacts)
    {
      contacts_left = max_contacts;
      contact_count = 0;
      contacts = contact_array;
    }

    void addContacts(unsigned int count)
    {
      contacts_left -= count;
      contact_count += count;
      contacts += count;
    }
  };

  class CollisionDetector
  {
  public:
    static unsigned int sphereAndHalfSpace(const CollisionSphere& sphere, const CollisionPlane& plane, CollisionData* data);
    static unsigned int sphereAndTruePlane(const CollisionSphere& sphere, const CollisionPlane& plane, CollisionData* data);
    static unsigned int sphereAndSphere(const CollisionSphere& one, const CollisionSphere& two, CollisionData* data);
    static unsigned int boxAndHalfSpace(const CollisionBox& box, const CollisionPlane& plane, CollisionData* data);
    static unsigned int boxAndBox(const CollisionBox& one, const CollisionBox& two, CollisionData* data);
    static unsigned int boxAndPoint(const CollisionBox& box, const Vector3& point, CollisionData* data);
    static unsigned int boxAndSphere(const CollisionBox& box, const CollisionSphere& sphere, CollisionData* data);
  };
};