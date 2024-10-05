#include <vulkan/vulkan.h>
#define GLFW_INCLUDE_VULKAN
#include <glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <cstring>

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

const std::vector<const char *> validation_layers = {"VK_LAYER_KHRONOS_validation"};

#ifdef NDEBUG
const bool enable_validation_layers = false;
#else
const bool enable_validation_layers = true;
#endif

class HelloTriangleApplication
{
private:
  GLFWwindow *window;
  VkInstance instance;

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
    createInstance();
  }

  void mainLoop()
  {
    while (!glfwWindowShouldClose(window))
    {
      glfwPollEvents();
    }
  }

  void cleanup()
  {
    vkDestroyInstance(instance, nullptr);
    glfwDestroyWindow(window);
    glfwTerminate();
  }

  void createInstance()
  {
    if (enable_validation_layers)
    {
      std::cout << "Debug mode\n";
    }
    else
    {
      std::cout << "No debug\n";
    }
    if (enable_validation_layers && !checkValidationLayerSupport())
    {
      throw std::runtime_error("validation layers requested, but not available!");
    }

    VkApplicationInfo app_info{};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = "Hello Triangle";
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName = "No Engine";
    app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.pApplicationInfo = &app_info;
    unsigned int glfw_extension_count = 0;
    const char **glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

    std::vector<const char *> required_extensions;

    for (unsigned int i = 0; i < glfw_extension_count; ++i)
    {
      required_extensions.emplace_back(glfw_extensions[i]);
    }

#ifdef __APPLE__
    required_extensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);

    create_info.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;

#endif

    create_info.enabledExtensionCount = (unsigned int)required_extensions.size();
    create_info.ppEnabledExtensionNames = required_extensions.data();

    if (vkCreateInstance(&create_info, nullptr, &instance) != VK_SUCCESS)
    {
      throw std::runtime_error("failed to create instance!");
    }
  }

  bool checkValidationLayerSupport()
  {
    unsigned int layer_count;
    vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
    std::vector<VkLayerProperties> available_layers(layer_count);
    vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

    for (char const *layer_name : validation_layers)
    {
      bool layer_found = false;
      for (const VkLayerProperties &layer_properties : available_layers)
      {
        if (std::strcmp(layer_name, layer_properties.layerName) == 0)
        {
          layer_found = true;
          break;
        }
      }
      if (!layer_found)
        return false;
    }

    return true;
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