#pragma once
#include <glm/glm.hpp>
#include "precision.h"
namespace BEPhysics
{
  class RigidBody
  {
  public:

    //characteristics
    real inverse_mass;
    Matrix3 inverse_inertia_tensor;
    real linear_damping;
    real angular_damping;
    bool can_sleep;

    //state
    Vector3 position;
    Quaternion orientation;
    Vector3 velocity;
    Vector3 angular_velocity;
    bool is_awake;

    //derived
    Matrix3 inverse_inertia_tensor_world;
    real motion;
    Matrix4 transform_matrix;
    Vector3 force_accumulator;
    Vector3 torque_accumulator;
    Vector3 acceleration;
    Vector3 last_frame_acceleration;

    //
    void calculateDerivedData();
    void integrate(real delta_time);
    real getMass() const;
    bool hasFiniteMass() const;
    Vector3 getPointInLocalSpace(const Vector3& point) const;
    Vector3 getPointInWorldSpace(const Vector3& point) const;
    Vector3 getDirectionInLocalSpace(const Vector3& direction) const;
    Vector3 getDirectionInWorldSpace(const Vector3& direction) const;
    void setAwake(const bool awake = true);
    void setCanSleep(const bool can_sleep);
    void clearAccumulators();
    void addForce(const Vector3& force);
    void addForceAtPoint(const Vector3& force, const Vector3& point);
    void addForceAtBodyPoint(const Vector3& force, const Vector3& point);
    void addTorque(const Vector3& torque);
  };
};

