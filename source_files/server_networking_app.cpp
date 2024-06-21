#include "server_networking_app.h"
#include <thread>
using namespace BEEngine;
void NetworkingServerApp::framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}

void NetworkingServerApp::mouse_callback(GLFWwindow *window, double xposIn, double yposIn)
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

void NetworkingServerApp::scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
  camera.processMouseScroll((float)yoffset);
}

void NetworkingServerApp::mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
  {
  }
}

void NetworkingServerApp::processInput(GLFWwindow *window)
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
  if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
  {
    if (crser)
    {
      crser = false;
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    else
    {
      crser = true;
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
  }
}

void NetworkingServerApp::initialize()
{
  crser=false;
  screen_width = 800;
  screen_height = 600;
  delta_time = 0.0;
  last_frame = 0.0;
  last_x = screen_width / 2;
  last_y = screen_height / 2;
  first_mouse = true;
  camera = BERender::Camera(glm::vec3(0.0, 2.0, 3.0));
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
#endif
  window = glfwCreateWindow(screen_width, screen_height, "Blobs", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "window creation failed";
    glfwTerminate();
  }
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "GLAD failed to initialize";
  }

  glViewport(0, 0, screen_width, screen_height);
  glfwSetWindowUserPointer(window, this);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback_friend);
  glfwSetCursorPosCallback(window, mouse_callback_friend);
  glfwSetScrollCallback(window, scroll_callback_friend);
  // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetMouseButtonCallback(window, mouse_button_callback_friend);
}

void NetworkingServerApp::run()
{
  initialize();
  mainLoop();
}

void NetworkingServerApp::mainLoop()
{

  // opengl setup
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)screen_width / (float)screen_height, 0.1f, 100.0f);
  glm::mat4 view = camera.getViewMatrix();
  glm::mat4 model = glm::mat4(1.0f);

  glm::vec3 light_position(0.5f, 0.5f, -3.3f);

  glm::vec3 other_position;

  float *float_pointer;

  BERender::TextRenderer text;
  text.loadCharacters();
  BERender::Shader text_shader_program("Shaders/VertexShader9.glsl", "Shaders/FragmentShader12.glsl");
  BERender::Shader shader_program("Shaders/VertexShader7.glsl", "Shaders/FragmentShader10.glsl");
  shader_program.use();
  shader_program.setVec3("point_light.position", light_position);
  shader_program.setVec3("point_light.ambient_color", 0.5f, 0.5f, 0.5f);
  shader_program.setVec3("point_light.diffuse_color", 0.8f, 0.8f, 0.8f);
  shader_program.setVec3("point_light.specular_color", 1.0f, 1.0f, 1.0f);
  shader_program.setFloat("point_lights.constant", 1.0f);
  shader_program.setFloat("point_lights.linear", 0.09f);
  shader_program.setFloat("point_lights.quadratic", 0.032f);

  BERender::Scene scene;
  // plane
  BERender::Geometry plane_geometry;
  plane_geometry.makePlane();
  glm::mat4 plane_model = glm::scale(glm::mat4(1.0f), glm::vec3(10.0f, 10.0f, 10.0f));
  BERender::Material plane_material;
  plane_material.diffuse_color = glm::vec3(1.0f, 1.0f, 1.0f);
  plane_material.ambient_color = glm::vec3(0.0f, 0.0f, 0.0f);
  plane_material.specular_color = glm::vec3(1.0f, 0.0f, 0.0f);

  // other model
  BERender::Geometry sphere_geometry;
  sphere_geometry.makeIcoSphere();
  glm::mat4 sphere_model = glm::translate(glm::mat4(1.0), other_position);

  // networking setup
  WSADATA wsaData;

  SOCKET ListenSocket = INVALID_SOCKET;
  SOCKET ClientSocket = INVALID_SOCKET;

  struct addrinfo *result = NULL;
  struct addrinfo hints;

  const char *sendbuf = (const char *)&camera.position;
  char recvbuf[DEFAULT_BUFLEN];
  int iResult;
  int iSendResult;
  int recvbuflen = DEFAULT_BUFLEN;
  // Initialize Winsock
  iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (iResult != 0)
  {
    printf("WSAStartup failed with error: %d\n", iResult);
    return;
  }

  ZeroMemory(&hints, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
  hints.ai_flags = AI_PASSIVE;

  // Resolve the server address and port
  iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
  if (iResult != 0)
  {
    printf("getaddrinfo failed with error: %d\n", iResult);
    WSACleanup();
    return;
  }

  // Create a SOCKET for the server to listen for client connections.
  ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
  if (ListenSocket == INVALID_SOCKET)
  {
    printf("socket failed with error: %ld\n", WSAGetLastError());
    freeaddrinfo(result);
    WSACleanup();
    return;
  }

  // Setup the TCP listening socket
  iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
  if (iResult == SOCKET_ERROR)
  {
    printf("bind failed with error: %d\n", WSAGetLastError());
    freeaddrinfo(result);
    closesocket(ListenSocket);
    WSACleanup();
    return;
  }

  freeaddrinfo(result);

  iResult = listen(ListenSocket, SOMAXCONN);
  if (iResult == SOCKET_ERROR)
  {
    printf("listen failed with error: %d\n", WSAGetLastError());
    closesocket(ListenSocket);
    WSACleanup();
    return;
  }

  ClientSocket = accept(ListenSocket, NULL, NULL);
  if (ClientSocket == INVALID_SOCKET)
  {
    printf("accept failed with error: %d\n", WSAGetLastError());
    closesocket(ListenSocket);
    WSACleanup();
    return;
  }
  // send/recieve
  std::thread t1([&]()
                 {while(true){iSendResult = send(ClientSocket, sendbuf, sizeof(glm::vec3), 0);
		if (iSendResult == SOCKET_ERROR)
		{
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
			return;
		}
		iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0)
		{
			printf("Bytes received: %d\n", iResult);
		}
		else if (iResult == 0)
			printf("Connection closing...\n");
		else
		{
			printf("recv failed with error: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
			return;
		}

		float_pointer = (float *)recvbuf;
		other_position.x = *float_pointer;
		++float_pointer;
		other_position.y = *float_pointer;
		++float_pointer;
		other_position.z = *float_pointer; } });

  while (!glfwWindowShouldClose(window))
  {
    double current_frame = glfwGetTime();
    delta_time = current_frame - last_frame;
    last_frame = current_frame;
    processInput(window);

    sphere_model = glm::translate(glm::mat4(1.0f), other_position);

    light_position.x = (float)sin(current_frame) * 3.0;
    light_position.z = (float)cos(current_frame) * 2.0;
    light_position.y = (float)5.0 + cos(current_frame) * 1.0;

    glClearColor(0.f, 0.f, 0.f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    shader_program.use();
    projection = glm::perspective(glm::radians(camera.zoom), (float)screen_width / (float)screen_height, 0.1f, 100.0f);
    view = camera.getViewMatrix();
    model = glm::mat4(1.0f);
    shader_program.setMat4("projection", projection);
    shader_program.setMat4("view", view);
    shader_program.setMat4("model", model);
    shader_program.setVec3("view_position", camera.position);
    shader_program.setVec3("point_light.position", light_position);
    scene.add(new BERender::DrawableGeometry(&plane_geometry, &plane_material, plane_model, shader_program));
    scene.add(new BERender::DrawableGeometry(&sphere_geometry, sphere_model, shader_program));
    scene.draw();
    scene.clear();
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwTerminate();
  // shutdown the connection since no more data will be sent
  iResult = shutdown(ClientSocket, SD_SEND);
  if (iResult == SOCKET_ERROR)
  {
    printf("shutdown failed with error: %d\n", WSAGetLastError());
    closesocket(ClientSocket);
    WSACleanup();
    return;
  }

  // cleanup
  closesocket(ClientSocket);
  WSACleanup();
}