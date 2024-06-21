#pragma once
#include "body.h"
#include "pfgen.h"
namespace BEPhysics
{
  class ForceGenerator
  {
  public:
    virtual void updateForce(RigidBody* body, real delta_time) = 0;
  };

  class Gravity :public ForceGenerator
  {
    Vector3 gravity;
  public:
    Gravity(const Vector3& gravity);
    virtual void updateForce(RigidBody* body, real delta_time);
  };

  class Spring : public ForceGenerator
  {
  public:
    Vector3 connection_point;
    Vector3 other_connection_point;
    RigidBody* other;
    real k;
    real l;
  public:
    Spring(const Vector3& local_connection_point, RigidBody* other, const Vector3& other_connection_point,
      real k, real l);
    virtual void updateForce(RigidBody* body, real delta_time);
  };

  class Explosion : public ForceGenerator, public ParticleForceGenerator
  {
    double time_passed;
  public:
    Vector3 detonation;
    real implosion_max_radius;
    real implosion_min_radius;
    real implosion_duration;
    real implosion_force;

    //thickness>=speed*min_frame_duration
    real shockwave_speed;
    real shockwave_thickness;
    real peack_concussion_force;
    real concussion_duration;


    real peak_convection_force;
    real chimney_radius;
    real chimney_height;
    real convection_duration;
  public:
    Explosion();
    virtual void updateForce(RigidBody* body, real delta_time);
    //virtual void applyForce(Particle* particle, real delta_time);

  };

  class Aero : public ForceGenerator
  {
  protected:
    Matrix3 tensor;
    Vector3 position;
    const Vector3* windspeed;
  public:
    Aero(const Matrix3& tensor, const Vector3& position, const Vector3* windspeed);
    virtual void updateForce(RigidBody* body, real delta_time);
  protected:
    void updateForceFromTensor(RigidBody* body, real duration, const Matrix3& tensor);
  };

  class AeroControl : public Aero
  {
  protected:
    Matrix3 max_tensor;
    Matrix3 min_tensor;

    real control_setting;

  private:
    Matrix3 getTensor();

  public:
    AeroControl(const Matrix3& base,
      const Matrix3& min, const Matrix3& max,
      const Vector3& position, const Vector3* wind_speed);

    void setControl(real value);

    virtual void updateForce(RigidBody* body, real duration);
  };


  class AngledAero : public Aero
  {

    Quaternion orientation;

  public:

    AngledAero(const Matrix3& tensor, const Vector3& position,
      const Vector3* windspeed);


    void setOrientation(const Quaternion& quat);


    virtual void updateForce(RigidBody* body, real duration);
  };


  class Buoyancy : public ForceGenerator
  {

    real max_depth;


    real volume;


    real water_height;

    real liquid_density;

    Vector3 centre_of_buoyancy;

  public:


    Buoyancy(const Vector3& cOfB,
      real max_depth, real volume, real water_height,
      real liquid_density = 1000.0f);


    virtual void updateForce(RigidBody* body, real duration);
  };


  class ForceRegistration
  {
  public:
    RigidBody* body;
    ForceGenerator* fg;

    ForceRegistration(RigidBody* body, ForceGenerator* fg);
  };

};
