#pragma once
#include "App.h"
namespace BEEngine
{
  class SpinningLightApp : public App
  {
    void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    void processInput(GLFWwindow* window);
    void initialize();
    void mainLoop();
  public:
    void run();
  };
};

