#pragma once
#include "body.h"
namespace BEPhysics
{
  class ContactResolver;
  class Contact
  {
    friend class ContactResolver;
  public:
    RigidBody* body[2];
    real friction;
    real restitution;
    Vector3 contact_point;
    Vector3 contact_normal;
    real penetration;
    void setBodyData(RigidBody* one, RigidBody* two, real friction, real restitution);
  protected:
    Matrix3 contact_to_world;
    Vector3 contact_velocity;
    real desired_delta_velocity;
    Vector3 relative_contact_position[2];
  protected:
    void calculateInternals(real delta_time);
    void swapBodies();
    void matchAwakeState();
    void calculateDesiredDeltaVelocity(real delta_time);
    Vector3 calculateLocalVelocity(unsigned int body_index, real delta_time);
    void calculateContactBasis();
    void applyImpulse(const Vector3& impulse, RigidBody* body, Vector3* velocity_change, Vector3* angular_velocity_change);
    void applyVelocityChange(Vector3 velocity_change[2], Vector3 angular_velocity_change[2]);
    void applyPositionChange(Vector3 linear_change[2], Vector3 angular_change[2], real penetration);
    Vector3 calculateFrictionlessImpulse(Matrix3* inverse_inertia_tensor);
    Vector3 calculateFrictionImpulse(Matrix3* inverse_inertia_tensor);
  };

  class ContactResolver
  {
  protected:
    unsigned int velocity_iterations;
    unsigned int position_iterations;
    real velocity_epsilon;
    real position_epsilon;
  public:
    unsigned int velocity_iterations_used;
    unsigned int position_iterations_used;
  private:
    bool valid_settings;
  public:
    ContactResolver(unsigned int iterations, real velocity_epsilon = (real)0.01,
      real position_epsilon = (real)0.01);
    ContactResolver(unsigned int velocity_iterations,
      unsigned int position_iterations,
      real velocity_epsilon = (real)0.01,
      real position_epsilon = (real)0.01);
    bool isValid()
    {
      return(velocity_iterations) > 0 &&
        (position_iterations) > 0 &&
        velocity_epsilon >= 0 &&
        position_epsilon >= 0;
    }

    void resolveContacts(Contact* contact_array, unsigned int nr_contacts, real delta_time);
  protected:
    void prepareContacts(Contact* contact_array, unsigned int nr_contacts, real delta_time);
    void adjustVelocities(Contact* contact_array, unsigned int nr_contacts, real delta_time);
    void adjustPositions(Contact* contact_array, unsigned int nr_contacts, real delta_time);
  };

  class ContactGenerator
  {
  public:
    virtual unsigned int addContact(Contact* contact, unsigned int limit) const = 0;
  };

};

