#include "collide_fine.h"
using namespace BEPhysics;
void CollisionPrimitive::calculateInternals()
{
  transform = body->transform_matrix * offset;
}

bool IntersectionTests::sphereAndHalfSpace(const CollisionSphere& sphere, const CollisionPlane& plane)
{
  real ball_distance = glm::dot(plane.direction, sphere.getAxis(3)) - sphere.radius;
  return ball_distance <= plane.offset;
}

bool IntersectionTests::sphereAndSphere(const CollisionSphere& one, const CollisionSphere& two)
{
  Vector3 midline = one.getAxis(3) - two.getAxis(3);
  return glm::length2(midline) < (one.radius + two.radius) * (one.radius + two.radius);
}

static inline real transformToAxis(const CollisionBox& box, const Vector3& axis)
{
  return box.half_size.x * real_abs(glm::dot(axis, box.getAxis(0))) +
    box.half_size.y * real_abs(glm::dot(axis, box.getAxis(1))) +
    box.half_size.z * real_abs(glm::dot(axis, box.getAxis(2)));
}

static inline bool overlapOnAxis(const CollisionBox& one, const CollisionBox& two, const Vector3& axis, const Vector3& to_center)
{
  real one_proj = transformToAxis(one, axis);
  real two_proj = transformToAxis(two, axis);
  real distance = real_abs(glm::dot(to_center, axis));
  return distance < (one_proj + two_proj);
}

#define TEST_OVERLAP(axis) overlapOnAxis(one, two, (axis), to_center)

bool IntersectionTests::boxAndBox(const CollisionBox& one, const CollisionBox& two)
{
  Vector3 to_center = two.getAxis(3) - two.getAxis(3);
  return(
    TEST_OVERLAP(one.getAxis(0)) &&
    TEST_OVERLAP(one.getAxis(1)) &&
    TEST_OVERLAP(one.getAxis(2)) &&

    TEST_OVERLAP(two.getAxis(0)) &&
    TEST_OVERLAP(two.getAxis(1)) &&
    TEST_OVERLAP(two.getAxis(2)) &&

    TEST_OVERLAP(glm::cross(one.getAxis(0), two.getAxis(0))) &&
    TEST_OVERLAP(glm::cross(one.getAxis(0), two.getAxis(1))) &&
    TEST_OVERLAP(glm::cross(one.getAxis(0), two.getAxis(2))) &&
    TEST_OVERLAP(glm::cross(one.getAxis(1), two.getAxis(0))) &&
    TEST_OVERLAP(glm::cross(one.getAxis(1), two.getAxis(1))) &&
    TEST_OVERLAP(glm::cross(one.getAxis(1), two.getAxis(2))) &&
    TEST_OVERLAP(glm::cross(one.getAxis(2), two.getAxis(0))) &&
    TEST_OVERLAP(glm::cross(one.getAxis(2), two.getAxis(1))) &&
    TEST_OVERLAP(glm::cross(one.getAxis(2), two.getAxis(2))));
}

#undef TEST_OVERLAP

bool IntersectionTests::boxAndHalfSpace(const CollisionBox& box, const CollisionPlane& plane)
{
  real projected_radius = transformToAxis(box, plane.direction);
  real box_distance = glm::dot(plane.direction, box.getAxis(3)) - projected_radius;
  return box_distance <= plane.offset;
}

////////////////////////////////////////////////////////////////////////

unsigned int CollisionDetector::sphereAndTruePlane(const CollisionSphere& sphere, const CollisionPlane& plane, CollisionData* data)
{
  if (data->contacts_left <= 0)
    return 0;

  Vector3 position = sphere.getAxis(3);
  real center_distance = glm::dot(plane.direction, position) - plane.offset;

  if (center_distance * center_distance > sphere.radius * sphere.radius)
    return 0;

  Vector3 normal = plane.direction;
  real penetration = -center_distance;
  if (center_distance < 0)
  {
    normal *= -1;
    penetration = -penetration;
  }
  penetration += sphere.radius;
  Contact* contact = data->contacts;
  contact->contact_normal = normal;
  contact->penetration = penetration;
  contact->contact_point = position - plane.direction * center_distance;
  contact->setBodyData(sphere.body, NULL, data->friction, data->restitution);
  data->addContacts(1);
  return 1;
}

unsigned int CollisionDetector::sphereAndHalfSpace(const CollisionSphere& sphere,
  const CollisionPlane& plane, CollisionData* data)
{
  if (data->contacts_left <= 0)
    return 0;

  Vector3 position = sphere.getAxis(3);
  real ball_distance = glm::dot(plane.direction, position) - sphere.radius - plane.offset;
  if (ball_distance >= 0)
    return 0;
  Contact* contact = data->contacts;
  contact->contact_normal = plane.direction;
  contact->penetration = -ball_distance;
  contact->contact_point = position - plane.direction * (ball_distance + sphere.radius);
  contact->setBodyData(sphere.body, NULL, data->friction, data->restitution);
  data->addContacts(1);
  return 1;
}

unsigned int CollisionDetector::sphereAndSphere(const CollisionSphere& one, const CollisionSphere& two, CollisionData* data)
{
  if (data->contacts_left <= 0)
    return 0;

  Vector3 position_one = one.getAxis(3);
  Vector3 position_two = two.getAxis(3);

  Vector3 midline = position_one - position_two;
  real size = glm::length(midline);

  if (size <= 0. || size >= one.radius + two.radius)
  {
    return 0;
  }

  Vector3 normal = glm::normalize(midline);
  Contact* contact = data->contacts;
  contact->contact_normal = normal;
  contact->contact_point = position_one + midline * 0.5;
  contact->penetration = (one.radius + two.radius - size);
  contact->setBodyData(one.body, two.body, data->friction, data->restitution);
  data->addContacts(1);
  return 1;
}

static inline real penetrationOnAxis(const CollisionBox& one, const CollisionBox& two,
  const Vector3& axis, const Vector3& to_center)
{
  real one_project = transformToAxis(one, axis);
  real two_project = transformToAxis(two, axis);
  real distance = real_abs(glm::dot(to_center, axis));
  return one_project + two_project - distance;
}

static inline bool tryAxis(const CollisionBox& one, const CollisionBox& two, Vector3 axis,
  const Vector3& to_center, unsigned int index, real& smallest_penetration, unsigned int& smallest_case)
{
  if (glm::length2(axis) < 0.0001)
    return true;
  axis = glm::normalize(axis);
  real penetration = penetrationOnAxis(one, two, axis, to_center);
  if (penetration < 0)
    return false;
  if (penetration < smallest_penetration)
  {
    smallest_penetration = penetration;
    smallest_case = index;
  }
  return true;
}

void fillPointFaceBoxBox(const CollisionBox& one, const CollisionBox& two, const Vector3& to_center,
  CollisionData* data, unsigned int best, real pen)
{
  Contact* contact = data->contacts;
  Vector3 normal = one.getAxis(best);
  if (glm::dot(one.getAxis(best), to_center) > 0)
  {
    normal = -normal;
  }
  Vector3 vertex = two.half_size;
  if (glm::dot(two.getAxis(0), normal) < 0)
    vertex.x = -vertex.x;
  if (glm::dot(two.getAxis(1), normal) < 0)
    vertex.y = -vertex.y;
  if (glm::dot(two.getAxis(2), normal) < 0)
    vertex.z = -vertex.z;

  contact->contact_normal = normal;
  contact->penetration = pen;
  contact->contact_point = two.getTransform() * Vector4(vertex, (real)1.f);
  contact->setBodyData(one.body, two.body, data->friction, data->restitution);
}

static inline Vector3 contactPoint(const Vector3& p_one, const Vector3& d_one, real one_size,
  const Vector3& p_two, const Vector3& d_two, real two_size, bool use_one)
{
  Vector3 to_st, c_one, c_two;
  real dp_sta_one, dp_sta_two, dp_one_two, sm_one, sm_two;
  real denom, mua, mub;

  sm_one = glm::length2(d_one);
  sm_two = glm::length2(d_two);
  dp_one_two = glm::dot(d_two, d_one);
  to_st = p_one - p_two;
  dp_sta_one = glm::dot(d_one, to_st);
  dp_sta_two = glm::dot(d_two, to_st);
  denom = sm_one * sm_two - dp_one_two * dp_one_two;
  if (real_abs(denom) < 0.0001f)
    return use_one ? p_one : p_two;

  mua = (dp_one_two * dp_sta_two - sm_two * dp_sta_one) / denom;
  mub = (sm_one * dp_sta_two - dp_one_two * dp_sta_one) / denom;

  if (mua > one_size || mua<-one_size || mub>two_size || mub < -two_size)
    return use_one ? p_one : p_two;
  else
  {
    c_one = p_one + d_one * mua;
    c_two = p_two + d_two * mub;
    return c_one * 0.5 + c_two * 0.5;
  }
}

#define CHECK_OVERLAP(axis,index) \
  if(!tryAxis(one,two,(axis),to_center,(index),pen,best)) return 0;

unsigned int CollisionDetector::boxAndBox(const CollisionBox& one, const CollisionBox& two, CollisionData* data)
{
  if (data->contacts_left <= 0)
    return 0;
  Vector3 to_center = two.getAxis(3) - one.getAxis(3);


  real pen = REAL_MAX;
  unsigned int best = 0xffffff;


  CHECK_OVERLAP(one.getAxis(0), 0);
  CHECK_OVERLAP(one.getAxis(1), 1);
  CHECK_OVERLAP(one.getAxis(2), 2);
  CHECK_OVERLAP(two.getAxis(0), 3);
  CHECK_OVERLAP(two.getAxis(1), 4);
  CHECK_OVERLAP(two.getAxis(2), 5);

  unsigned best_single_axis = best;

  CHECK_OVERLAP(glm::cross(one.getAxis(0), two.getAxis(0)), 6);
  CHECK_OVERLAP(glm::cross(one.getAxis(0), two.getAxis(1)), 7);
  CHECK_OVERLAP(glm::cross(one.getAxis(0), two.getAxis(2)), 8);
  CHECK_OVERLAP(glm::cross(one.getAxis(1), two.getAxis(0)), 9);
  CHECK_OVERLAP(glm::cross(one.getAxis(1), two.getAxis(1)), 10);
  CHECK_OVERLAP(glm::cross(one.getAxis(1), two.getAxis(2)), 11);
  CHECK_OVERLAP(glm::cross(one.getAxis(2), two.getAxis(0)), 12);
  CHECK_OVERLAP(glm::cross(one.getAxis(2), two.getAxis(1)), 13);
  CHECK_OVERLAP(glm::cross(one.getAxis(2), two.getAxis(2)), 14);

  if (best < 3)
  {
    fillPointFaceBoxBox(one, two, to_center, data, best, pen);
    data->addContacts(1);
    return 1;
  }
  else if (best < 6)
  {
    fillPointFaceBoxBox(two, one, -to_center, data, best - 3, pen);
    data->addContacts(1);
    return 1;
  }
  else
  {
    best -= 6;
    unsigned int one_axis_index = best / 3;
    unsigned int two_axis_index = best % 3;
    Vector3 one_axis = one.getAxis(one_axis_index);
    Vector3 two_axis = two.getAxis(two_axis_index);
    Vector3 axis = glm::cross(one_axis, two_axis);
    axis = glm::normalize(axis);

    if (glm::dot(axis, to_center) > 0)
      axis = -axis;

    Vector3 pt_on_one_edge = one.half_size;
    Vector3 pt_on_two_edge = two.half_size;
    for (unsigned int i = 0; i < 3; i++)
    {
      if (i == one_axis_index)
        pt_on_one_edge[i] = 0;
      else if (glm::dot(one.getAxis(i), axis) > 0)
        pt_on_one_edge[i] = -pt_on_one_edge[i];

      if (i == two_axis_index)
        pt_on_two_edge[i] = 0;
      else if (glm::dot(two.getAxis(i), axis) < 0)
        pt_on_two_edge[i] = -pt_on_two_edge[i];

    }

    pt_on_one_edge = one.transform * Vector4(pt_on_one_edge, 1.);
    pt_on_two_edge = two.transform * Vector4(pt_on_two_edge, 1.);

    Vector3 vertex = contactPoint(pt_on_one_edge, one_axis, one.half_size[one_axis_index],
      pt_on_two_edge, two_axis, two.half_size[two_axis_index], best_single_axis > 2);

    Contact* contact = data->contacts;
    contact->penetration = pen;
    contact->contact_normal = axis;
    contact->contact_point = vertex;
    contact->setBodyData(one.body, two.body, data->friction, data->restitution);
    data->addContacts(1);
    return 1;

  }
  return 0;
}

#undef CHECK_OVERLAP

unsigned int CollisionDetector::boxAndPoint(const CollisionBox& box, const Vector3& point, CollisionData* data)
{
  if (data->contacts_left <= 0)
    return 0;
  Vector3 rel_pt = glm::inverse(box.transform) * Vector4(point, 1.0);
  Vector3 normal;
  real min_depth = box.half_size.x - real_abs(rel_pt.x);
  if (min_depth < 0)
    return 0;
  normal = box.getAxis(0) * ((rel_pt.x < 0) ? -1 : 1);

  real depth = box.half_size.y - real_abs(rel_pt.y);
  if (depth < 0)
    return 0;
  else if (depth < min_depth)
  {
    min_depth = depth;
    normal = box.getAxis(1) * ((rel_pt.y < 0) ? -1 : 1);
  }

  depth = box.half_size.z - real_abs(rel_pt.z);
  if (depth < 0)
    return 0;
  else if (depth < min_depth)
  {
    min_depth = depth;
    normal = box.getAxis(2) * ((rel_pt.z < 0) ? -1 : 1);
  }

  Contact* contact = data->contacts;
  contact->contact_normal = normal;
  contact->contact_point = point;
  contact->penetration = min_depth;
  contact->setBodyData(box.body, NULL, data->friction, data->restitution);
  data->addContacts(1);
  return 1;

}

unsigned int CollisionDetector::boxAndSphere(const CollisionBox& box, const CollisionSphere& sphere, CollisionData* data)
{
  if (data->contacts_left <= 0)
    return 0;
  Vector3 center = sphere.getAxis(3);
  Vector3 rel_center = glm::inverse(box.transform) * Vector4(center, 1.);
  if (real_abs(rel_center.x) - sphere.radius > box.half_size.x ||
    real_abs(rel_center.y) - sphere.radius > box.half_size.y ||
    real_abs(rel_center.z) - sphere.radius > box.half_size.z)
  {
    return 0;
  }

  Vector3 closest_point(0);
  real dist;


  dist = rel_center.x;
  if (dist > box.half_size.x)
    dist = box.half_size.x;
  if (dist < -box.half_size.x)
    dist = -box.half_size.x;
  closest_point.x = dist;

  dist = rel_center.y;
  if (dist > box.half_size.y)
    dist = box.half_size.y;
  if (dist < -box.half_size.y)
    dist = -box.half_size.y;
  closest_point.y = dist;

  dist = rel_center.z;
  if (dist > box.half_size.z)
    dist = box.half_size.z;
  if (dist < -box.half_size.z)
    dist = -box.half_size.z;
  closest_point.z = dist;


  dist = glm::length2(closest_point - rel_center);
  if (dist > sphere.radius * sphere.radius)
    return 0;

  Vector3 closest_point_world = box.transform * Vector4(closest_point, 1.);

  Contact* contact = data->contacts;
  contact->contact_normal = glm::normalize(closest_point_world - center);
  contact->contact_point = closest_point_world;
  contact->penetration = sphere.radius - real_sqrt(dist);
  contact->setBodyData(box.body, sphere.body, data->friction, data->restitution);
  data->addContacts(1);
  return 1;

}

unsigned int CollisionDetector::boxAndHalfSpace(const CollisionBox& box, const CollisionPlane& plane, CollisionData* data)
{
  if (data->contacts_left <= 0)
    return 0;

  if (!IntersectionTests::boxAndHalfSpace(box, plane))
    return 0;

  static real mults[8][3] = { {1,1,1},{-1,1,1},{1,-1,1},{-1,-1,1},
                             {1,1,-1},{-1,1,-1},{1,-1,-1},{-1,-1,-1} };

  Contact* contact = data->contacts;
  unsigned contacts_used = 0;
  for (unsigned int i = 0; i < 8; i++)
  {
    Vector3 vertex_pos(mults[i][0], mults[i][1], mults[i][2]);
    vertex_pos = vertex_pos * box.half_size;
    vertex_pos = box.transform * Vector4(vertex_pos, 1.);

    real vertex_distance = glm::dot(vertex_pos, plane.direction);
    if (vertex_distance <= plane.offset)
    {
      contact->contact_point = vertex_pos + plane.direction * (vertex_distance - plane.offset);
      contact->contact_normal = plane.direction;
      contact->penetration = plane.offset - vertex_distance;
      contact->setBodyData(box.body, NULL, data->friction, data->restitution);
      contact++;
      contacts_used++;
      if (contacts_used == (unsigned int)data->contacts_left)
      {
        data->addContacts(contacts_used);
        return contacts_used;
      }
    }
    data->addContacts(contacts_used);
  }
  return contacts_used;
}