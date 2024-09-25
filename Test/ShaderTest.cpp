#include <glad/glad.h>
#include <Camera.h>
#include <Shader.h>
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

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
  camera.processMouseScroll((float)yoffset);
}

int main()
{

  float corners[] = {0., 0., -1.,
                     1., 0., -1.,
                     1., -1., -1.};
  unsigned int triangle[] = {0, 2, 1};

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
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  glm::mat4 projection = glm::perspective(glm::radians(camera.zoom),
                                          (float)screen_width / (float)screen_height,
                                          0.1f, 100.0f);

  glm::mat4 view = camera.getViewMatrix();

  glm::mat4 model = glm::mat4(1.0f);

  BERender::Shader shader_program("Resources/Shaders/ShaderTest/VertexShader.glsl",
                                  "Resources/Shaders/ShaderTest/FragmentShader.glsl");

  shader_program.use();
  shader_program.setMat4("projection", projection);
  shader_program.setMat4("model", model);

  unsigned int VAO, EBO, VBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), &corners, GL_STATIC_DRAW);
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(unsigned int), &triangle, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

  while (!glfwWindowShouldClose(window))
  {
    double current_frame = glfwGetTime();
    delta_time = current_frame - last_frame;
    last_frame = current_frame;
    processInput(window);

    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    view = camera.getViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(camera.zoom),
                                            (float)screen_width / (float)screen_height,
                                            0.1f, 100.0f);
    shader_program.setMat4("view", view);
    shader_program.setMat4("projection", projection);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
    glfwSwapBuffers(window);
    glfwPollEvents();
    std::cout << camera.position.x << ' ' << camera.position.y << ' ' << camera.position.z << '\n';
  }
  glfwTerminate();
  return 0;
}