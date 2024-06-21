#pragma once
#include <float.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
namespace BEPhysics
{
#if 1

  #define SINGLE_PRECISION
  //typedefs
  typedef float real;
  typedef glm::vec3 Vector3;
  typedef glm::vec4 Vector4;
  typedef glm::mat3 Matrix3;
  typedef glm::mat4 Matrix4;
  typedef glm::quat Quaternion;
  //#defines
  #define real_sqrt sqrtf
  #define real_abs fabsf
  #define real_sin sinf
  #define real_cos cosf
  #define real_exp expf
  #define real_pow powf
  #define real_mod fmodf
  #define real_epsilon FLT_EPSILON
  #define REAL_MAX FLT_MAX
  #define R_PI 3.14159f

#else

#define DOUBLE_PRECISION
  //typedefs
  typedef double real;
  typedef glm::dvec3 Vector3;
  typedef glm::dvec4 Vector4;
  typedef glm::dmat3 Matrix3;
  typedef glm::dmat4 Matrix4;
  typedef glm::dquat Quaternion;
  //#defines
  #define real_sqrt sqrt
  #define real_abs fabs
  #define real_sin sin
  #define real_cos cos
  #define real_exp exp
  #define real_pow pow
  #define real_mod fmod
  #define real_epsilon = DBL_EPSILON
  #define REAL_MAX DBL_MAX
  #define R_PI 3.14159265358979

#endif

};