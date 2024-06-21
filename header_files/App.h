#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>
#include "BERender.h"
#include "BEPhysics.h"
#include <vector>

namespace BEEngine
{
  void framebuffer_size_callback_friend(GLFWwindow* window, int width, int height);

  void mouse_callback_friend(GLFWwindow* window, double xposIn, double yposIn);

  void scroll_callback_friend(GLFWwindow* window, double xoffset, double yoffset);

  void mouse_button_callback_friend(GLFWwindow* window, int button, int action, int mods);

  class App
  {
    protected:
    //scene
    BERender::Scene scene;
    BERender::Camera camera;
    //screen
    GLFWwindow* window;
    unsigned int screen_width;
    unsigned int screen_height;
    //input
    float last_x;
    float last_y;
    bool first_mouse;
    //dt
    BEPhysics::real delta_time;
    BEPhysics::real last_frame;

    //input functions
    friend void framebuffer_size_callback_friend(GLFWwindow* window, int width, int height);
    virtual void framebuffer_size_callback(GLFWwindow* window, int width, int height) {}

    friend void mouse_callback_friend(GLFWwindow* window, double xposIn, double yposIn);
    virtual void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {}

    friend void scroll_callback_friend(GLFWwindow* window, double xoffset, double yoffset);
    virtual void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {}

    friend void mouse_button_callback_friend(GLFWwindow* window, int button, int action, int mods);
    virtual void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {}

    virtual void processInput(GLFWwindow* window) {}

    // // // // // //
    virtual void initialize() = 0;
    virtual void mainLoop() = 0;
  public:
    virtual void run() = 0;
  };

  class ParticlePhysicsApp : public App
  {
  protected:
    std::vector<BEPhysics::Particle> particles;
    std::vector<BEPhysics::ParticleForceRegistration> force_generator;
    std::vector<BEPhysics::ParticleContactGenerator*> contact_generator;
    BEPhysics::ParticleContact* contact;
    BEPhysics::ParticleContactResolver contact_resolver;
  };

  class RigidBodyPhysicsApp : public App
  {
  protected:
    std::vector<BEPhysics::RigidBody> bodies;
    std::vector<BEPhysics::ForceRegistration> force_generators;
  };
};

