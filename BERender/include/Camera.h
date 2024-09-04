#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>

#include <vector>

namespace BERender
{
  using namespace glm;
  enum Camera_Movement{FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN};
}

#endif //CAMERA_H