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
  enum Camera_Movement
  {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
  };

  const float YAW = -90.0f;
  const float PITCH = 0.0f;
  const float SPEED = 2.5f;
  const float SENSITIVITY = 0.1f;
  const float ZOOM = 45.0f;

  class Camera
  {
  public:
    vec3 position;
    vec3 front;
    vec3 up;
    vec3 right;
    vec3 world_up;

    float yaw;
    float pitch;

    float movement_speed;
    float mouse_sensitivity;
    float zoom;

    Camera(vec3 position = vec3(0, 0, 0), vec3 up = vec3(0, 1, 0), float yaw = YAW, float pitch = PITCH) :
      front(vec3(0, 0, -1)), movement_speed(SPEED), mouse_sensitivity(SENSITIVITY), zoom(ZOOM)
    {
      this->position = position;
      world_up = up;
      this->yaw = yaw;
      this->pitch = pitch;
      updateCameraVectors();
    }

    mat4 getViewMatrix()
    {
      return lookAt(position, position + front, up);
    }

    void processKeyboard(Camera_Movement direction, float deltaTime)
    {
      float velocity = movement_speed * deltaTime;
      if (direction == FORWARD)
      {
        position += velocity * front;
      }
      if (direction == BACKWARD)
      {
        position -= velocity * front;
      }
      if (direction == LEFT)
      {
        position -= velocity * right;
      }
      if (direction == RIGHT)
      {
        position += velocity * right;
      }
      if (direction == UP)
      {
        position += velocity * up;
      }
      if (direction == DOWN)
      {
        position -= velocity * up;
      }
    }

    void processMouseMovement(float xoffset, float yoffset, bool constraint_pitch = true)
    {
      xoffset *= mouse_sensitivity;
      yoffset *= mouse_sensitivity;

      yaw += xoffset;
      pitch += yoffset;

      if (constraint_pitch)
      {
        if (pitch > 89.0f)
          pitch = 89.0;
        if (pitch < -89.0f)
          pitch = -89.0f;
      }
      updateCameraVectors();
    }

    void processMouseScroll(float yoffset)
    {
      zoom -= yoffset;
      if (zoom < 1.0)
        zoom = 1;
      if (zoom > 45.0)
        zoom = 45.0;
    }
  private:
    void updateCameraVectors()
    {
      vec3 temp_front;
      temp_front.x = cos(radians(yaw)) * cos(radians(pitch));
      temp_front.y = sin(radians(pitch));
      temp_front.z = sin(radians(yaw)) * cos(radians(pitch));
      front = normalize(temp_front);
      right = normalize(cross(front, world_up));
      up = normalize(cross(right, front));
    }
  };
};
#endif