#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>
#include <vector>
#include <Directions.h>

namespace BERender
{
  class Camera
  {
  public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 world_up;

    float yaw;
    float pitch;

    float movement_speed;
    float mouse_sensitivity;
    float zoom;

    Camera(glm::vec3 position = glm::vec3(0, 0, 0), glm::vec3 up = glm::vec3(0, 1, 0), float yaw = -90.0f, float pitch = 0.0f);
    glm::mat4 getViewMatrix();
    void processKeyboard(Direction direction, float dt);
    void processMouseMovement(float xoffset, float yoffset, bool constraint_pitch = false);
    void processMouseScroll(float yoffset);

  private:
    void updateCameraVectors();
  };
}

#endif // CAMERA_H