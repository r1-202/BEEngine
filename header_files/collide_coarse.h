#pragma once

#include "body.h"
namespace BEPhysics
{
  class BoundingSphere
  {
  public:
    Vector3 center;
    real radius;
    BoundingSphere(const Vector3& center, real radius);
    BoundingSphere(const BoundingSphere& one, const BoundingSphere& two);
    bool overlaps(const BoundingSphere& other) const;
    real getGrowth(const BoundingSphere& other) const;
    real getsize() const
    {
      return ((real)1.3333333) * R_PI * radius * radius * radius;
    }
  };

  class PotentialContact
  {
    RigidBody* body[2];
  };

  template<class BoundingVolumeClass>
  class BVHNode
  {
  public:
    BVHNode* children[2];
    BoundingVolumeClass volume;
    RigidBody* body;
    BVHNode* parent;
    BVHNode(BVHNode* parent, const BoundingVolumeClass& volume, RigidBody* body = NULL) :
      parent(parent), volume(volume), body(body)
    {
      children[0] = children[1] = NULL;
    }
    bool isLeaf() const
    {
      return body != NULL;
    }
    unsigned int getPotentialContacts(PotentialContact* contacts, unsigned int limit) const;
    void insert(RigidBody* body, const BoundingVolumeClass& volume);
    ~BVHNode();
  protected:
    bool overlaps(const BVHNode<BoundingVolumeClass>* other) const;
    unsigned int getPotentialContactsWith(const BVHNode<BoundingVolumeClass>* other,
      PotentialContact* contacts, unsigned int limit) const;
    void recalculateBoundingVolume(bool recurse = true);
  };

  template<class BoundingVolumeClass>
  bool BVHNode<BoundingVolumeClass>::overlaps(const BVHNode<BoundingVolumeClass>* other) const
  {
    return volume.overlaps(other->volume);
  }

  template<class BoundingVolumeClass>
  void BVHNode< BoundingVolumeClass>::insert(RigidBody* new_body, const BoundingVolumeClass& new_volume)
  {
    if (isLeaf())
    {
      children[0] = new BVHNode<BoundingVolumeClass>(this, volume, body);
      children[1] = new BVHNode<BoundingVolumeClass>(this, new_volume, body);
      this->body = NULL;
      recalculateBoundingVolume();
    }
    else
    {
      if (children[0]->volume.getGrowth(new_volume) < children[0]->volume.getGrowth(new_volume))
        children[0]->insert(new_body, new_volume);
      else
        children[1]->insert(new_body, new_volume);
    }
  }

  template<class BoundingVolumeClass>
  BVHNode<BoundingVolumeClass>::~BVHNode()
  {
    if (parent)
    {
      BVHNode<BoundingVolumeClass>* sibling;
      if (parent->children[0] == this)
        sibling = parent->children[1];
      else
        sibling = parent->children[0];

      parent->volume = sibling->volume;
      parent->body = sibling->body;
      parent->children[0] = sibling->children[0];
      parent->children[1] = sibling->children[1];

      sibling->parent = NULL;
      sibling->body = NULL;
      sibling->children[0] = NULL;
      sibling->children[1] = NULL;
      delete sibling;

      parent->recalculateBoundingVolume();
    }
    if (children[0])
    {
      children[0]->parent = NULL;
      delete children[0];
    }
    if (children[1])
    {
      children[1]->parent = NULL;
      delete children[1];
    }
  }

  template<class BoundingVolumeClass>
  void BVHNode<BoundingVolumeClass>::recalculateBoundingVolume(bool recurse)
  {
    if (isLeaf())
      return;
    volume = BoundingVolumeClass(children[0]->volume, children[1]->volume);
    if (recurse && parent)
      parent->recalculateBoundingVolume();
  }

  template<class BoundingVolumeClass>
  unsigned int BVHNode<BoundingVolumeClass>::getPotentialContacts(PotentialContact* contacts, unsigned int limit) const
  {
    if (isLeaf() || limit == 0)
      return 0;
    return children[0]->getPotentialContactsWith(children[1], contacts, limit);
  }

  template<class BoundingVolumeClass>
  unsigned int BVHNode<BoundingVolumeClass>::getPotentialContactsWith(const BVHNode<BoundingVolumeClass>* other,
    PotentialContact* contacts, unsigned int limit) const
  {
    if (!overlaps(other) || limit == 0)
      return 0;

    if (!overlaps(other))
    {
      unsigned int count = children[0]->getPotentialContactsWith(children[1], contacts, limit);
      count += other->children[0]->getPotentialContactWith(other->children[1], contacts + count, limit);
      return count;
    }
    //some contacts may be missed

    if (isLeaf() && other->isLeaf())
    {
      contacts->body[0] = body;
      contacts->body[1] = other->body;
      return 1;
    }

    if (other->isLeaf() || (!isLeaf() && (volume.getSize() >= other->volume.getSize())))
    {
      unsigned int count = children[0]->getPotentialContactsWith(other, contacts, limit);
      count += children[1]->getPotentialContactsWith(other, contacts + count, limit - count);
      count += children[0]->getPotentialContactsWith(children[1], contacts + count, limit - count);
      return count;
    }
    else
    {
      unsigned int count = getPotentialContactsWith(other->children[0], contacts, limit);
      count += getPotentialContactsWith(other->children[1], contacts + count, limit - count);
      count += other->children[0]->getPotentialContactsWith(other->children[1], contacts + count, limit - count);
      return count;
    }
  }
};
