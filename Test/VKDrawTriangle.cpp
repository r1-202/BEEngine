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

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
                                      const VkDebugUtilsMessengerCreateInfoEXT *p_create_info,
                                      const VkAllocationCallbacks *p_allocator,
                                      VkDebugUtilsMessengerEXT *p_debug_messenger)
{
  auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance,
                                                                        "vkCreateDebugUtilsMessengerEXT");
  if (func != nullptr)
  {
    return func(instance, p_create_info, p_allocator, p_debug_messenger);
  }
  else
  {
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance,
                                   VkDebugUtilsMessengerEXT debug_messenger,
                                   const VkAllocationCallbacks *p_allocator)
{
  auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance,
                                                                         "vkDestroyDebugUtilsMessengerEXT");
  if (func != nullptr)
  {
    func(instance, debug_messenger, p_allocator);
  }
}

class HelloTriangleApplication
{
private:
  GLFWwindow *window;
  VkInstance instance;
  VkDebugUtilsMessengerEXT debug_messenger;

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
    setupDebugMessenger();
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
    if (enable_validation_layers)
    {
      DestroyDebugUtilsMessengerEXT(instance, debug_messenger, nullptr);
    }

    vkDestroyInstance(instance, nullptr);
    glfwDestroyWindow(window);
    glfwTerminate();
  }

  void createInstance()
  {
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

    auto extensions = getRequiredExtensions();

#ifdef __APPLE__
    create_info.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif

    create_info.enabledExtensionCount = (unsigned int)extensions.size();
    create_info.ppEnabledExtensionNames = extensions.data();

    VkDebugUtilsMessengerCreateInfoEXT debug_create_info{};

    if (enable_validation_layers)
    {
      create_info.enabledLayerCount = (unsigned int)validation_layers.size();
      create_info.ppEnabledLayerNames = validation_layers.data();

      populateDebugMessengerCreateInfo(debug_create_info);
      create_info.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debug_create_info;
    }
    else
    {
      create_info.enabledLayerCount = 0;
      create_info.pNext = nullptr;
    }

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

  std::vector<char const *> getRequiredExtensions()
  {
    unsigned int glfw_extension_count = 0;
    char const **glfw_extensions;
    glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);
    std::vector<char const *> extensions(glfw_extensions, glfw_extensions + glfw_extension_count);
    if (enable_validation_layers)
    {
      extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

#ifdef __APPLE__
    extensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
#endif

    return extensions;
  }

  static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
      VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
      VkDebugUtilsMessageTypeFlagsEXT message_type,
      const VkDebugUtilsMessengerCallbackDataEXT *p_callback_data,
      void *p_user_data)
  {
    std::cerr << "validation layer: " << p_callback_data->pMessage << std::endl;
    return VK_FALSE;
  }

  void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &create_info)
  {
    create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                  VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                  VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                              VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                              VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    create_info.pfnUserCallback = debugCallback;
  }

  void setupDebugMessenger()
  {
    if (!enable_validation_layers)
      return;
    VkDebugUtilsMessengerCreateInfoEXT create_info{};
    populateDebugMessengerCreateInfo(create_info);
    if (CreateDebugUtilsMessengerEXT(instance, &create_info, nullptr, &debug_messenger) != VK_SUCCESS)
    {
      throw std::runtime_error("failed to set up debug messenger!");
    }
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