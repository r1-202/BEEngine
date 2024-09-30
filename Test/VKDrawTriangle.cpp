#include <vulkan/vulkan.h>
#define GLFW_INCLUDE_VULKAN
#include <glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

class HelloTriangleApplication
{
private:
  GLFWwindow *window;

public:
  void run()
  {
    initWindow();
    initVulkan();
    mainLoop();
    cleanup();
  }

private:
  void initWindow()
  {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
  }
  void initVulkan()
  {
  }
  void mainLoop()
  {
    while(!glfwWindowShouldClose(window))
    {
      glfwPollEvents();
    }
  }
  void cleanup()
  {
    glfwDestroyWindow(window);
    glfwTerminate();
  }
};

int main()
{
  HelloTriangleApplication app;
  try
  {
    app.run();
  }
  catch (const std::exception &e)
  {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}