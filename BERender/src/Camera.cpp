#include "Camera.h"

using namespace BERender;

Camera::Camera(glm::vec3 position = glm::vec3(0, 0, 0), glm::vec3 up = glm::vec3(0, 1, 0), float yaw = -90.0f, float pitch = 0.0f) : front(glm::vec3(0, 0, -1)), movement_speed(2.5f), mouse_sensitivity(0.1f), zoom(45.0f)
{
  this->position = position;
  world_up = up;
  this->yaw = yaw;
  this->pitch = pitch;
  updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix()
{
  return glm::lookAt(position, position + front, up);
}

void Camera::processKeyboard(Direction direction, float dt)
{
  float velocity = movement_speed * dt;

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
    position += velocity * up;
  }
}
void Camera::processMouseMovement(float xoffset, float yoffset, bool constraint_pitch = false)
{
  xoffset *= mouse_sensitivity;
  yoffset *= mouse_sensitivity;

  yaw += xoffset;
  pitch += yoffset;

  if (constraint_pitch)
  {
    if (pitch > 89.0f)
      pitch = 89.0f;
    if (pitch < -89.0f)
      pitch = -89.0f;
  }
  updateCameraVectors();
}

void Camera::processMouseScroll(float yoffset)
{
  zoom -= yoffset;
  if (zoom < 1.0f)
    zoom = 1;
  if (zoom > 45.0f)
    zoom = 45.0;
}

void Camera::updateCameraVectors()
{
  glm::vec3 temp_front;
  temp_front.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
  temp_front.y = glm::sin(glm::radians(pitch));
  temp_front.z = glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
  front = glm::normalize(temp_front);
  right = glm::normalize(glm::cross(front, world_up));
  up = glm::normalize(glm::cross(right, front));
}