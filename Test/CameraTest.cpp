#include <glad/glad.h>
#include <Camera.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glfw3.h>

BERender::Camera camera;
bool first_mouse;
int screen_width;
int screen_height;
double delta_time;
double last_frame;
int last_x;
int last_y;

void processInput(GLFWwindow *window)
{
  system("cls");
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, true);
  }
  const float cameraSpeed = 2.5f * delta_time;
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
  {
    camera.processKeyboard(BERender::FORWARD, delta_time);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
  {
    camera.processKeyboard(BERender::BACKWARD, delta_time);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
  {
    camera.processKeyboard(BERender::LEFT, delta_time);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
  {
    camera.processKeyboard(BERender::RIGHT, delta_time);
  }
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
  {
    camera.processKeyboard(BERender::UP, delta_time);
  }
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
  {
    camera.processKeyboard(BERender::DOWN, delta_time);
  }
  std::cout << '(' << camera.position.x << ',' << camera.position.y << ',' << camera.position.z << ')' << '\n';
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn)
{
  float xpos = (float)xposIn;
  float ypos = (float)yposIn;
  if (first_mouse)
  {
    last_x = xpos;
    last_y = ypos;
    first_mouse = false;
  }
  float xoffset = xpos - last_x;
  float yoffset = last_y - ypos;
  last_x = xpos;
  last_y = ypos;

  camera.processMouseMovement(xoffset, yoffset);
}

int main()
{
  screen_width = 800;
  screen_height = 600;
  delta_time = 0.0;
  last_frame = 0.0;
  last_x = screen_width / 2;
  last_y = screen_height / 2;
  first_mouse = true;
  camera = BERender::Camera();
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
#endif
  GLFWwindow *window = glfwCreateWindow(screen_width, screen_height, "CameraTest", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "window creation failed.";
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "GLAD failed to initialize";
    return -1;
  }
  glViewport(0, 0, screen_width, screen_height);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  //glfwSetCursorPosCallback(window, mouse_callback);
  while (!glfwWindowShouldClose(window))
  {
    double current_frame = glfwGetTime();
    delta_time = current_frame - last_frame;
    last_frame = current_frame;
    processInput(window);
    glfwPollEvents();
  }
  glfwTerminate();
  return 0;
}