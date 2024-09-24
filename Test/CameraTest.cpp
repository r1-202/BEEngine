#include <glad/glad.h>
#include <Camera.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glfw3.h>

void processInput(GLFWwindow* window, float delta_time, BERender::Camera &camera)
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
  std::cout<<'('<<camera.position.x<<','<<camera.position.y<<','<<camera.position.z<<')'<<'\n';
}

int main()
{
  int screen_width = 800;
  int screen_height = 600;
  double delta_time = 0.0;
  double last_frame = 0.0;
  int last_x = screen_width / 2;
  int last_y = screen_height / 2;
  bool first_mouse = true;
  BERender::Camera camera = BERender::Camera();
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
#endif
  GLFWwindow *window = glfwCreateWindow(screen_width, screen_height, "CameraTest", NULL, NULL);
  if(window == NULL)
  {
    std::cout<<"window creation failed.";
    glfwTerminate();
  }
  glfwMakeContextCurrent(window);
  if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout<<"GLAD failed to initialize";
  }
  glViewport(0,0,screen_width,screen_height);
  while(!glfwWindowShouldClose(window))
  {
    double current_frame = glfwGetTime();
    delta_time = current_frame - last_frame;
    last_frame = current_frame;
    processInput(window,delta_time, camera);
  }
}