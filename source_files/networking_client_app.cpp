#include "networking_client_app.h"
using namespace BEEngine;
void NetworkingClientApp::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void NetworkingClientApp::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
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

void NetworkingClientApp::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.processMouseScroll((float)yoffset);
}

void NetworkingClientApp::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{

	}
}

void NetworkingClientApp::processInput(GLFWwindow* window)
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

void NetworkingClientApp::initialize()
{
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
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetMouseButtonCallback(window, mouse_button_callback_friend);
}

void NetworkingClientApp::mainLoop()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)screen_width / (float)screen_height, 0.1f, 100.0f);
	glm::mat4 view = camera.getViewMatrix();
	glm::mat4 model = glm::mat4(1.0f);

	glm::vec3 light_position(0.5f, 0.5f, -3.3f);

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
	BERender::Geometry plane_geometry;
	plane_geometry.makePlane();
	while (!glfwWindowShouldClose(window))
	{
		double current_frame = glfwGetTime();
		delta_time = current_frame - last_frame;
		last_frame = current_frame;
		processInput(window);

		light_position.x = (float)sin(current_frame) * 3.0;
		light_position.z = (float)cos(current_frame) * 2.0;
		light_position.y = (float)5.0 + cos(current_frame) * 1.0;


		glClearColor(0.f, 0.f, 0.f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader_program.use();
		projection = glm::perspective(glm::radians(camera.zoom), (float)screen_width / (float)screen_height, 0.1f, 100.0f);
		view = camera.getViewMatrix();
		model = glm::mat4(1.0f);
		shader_program.setMat4("projection", projection);
		shader_program.setMat4("view", view);
		shader_program.setMat4("model", model);
		shader_program.setVec3("view_position", camera.position);
		shader_program.setVec3("point_light.position", light_position);
		scene.add(new BERender::DrawableGeometry(&plane_geometry, model, shader_program));

		scene.draw();
		text.RenderText(text_shader_program, "connecting to server", 25.0f, 25.0f, 1.0f, glm::vec3(1.0f,0.0f,0.0f));
		scene.clear();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
}

void NetworkingClientApp::run()
{
	initialize();
	mainLoop();
}

void NetworkingClientApp::run(int argc, char** argv)
{
	initialize();
	mainLoop(argc, argv);
}

void NetworkingClientApp::mainLoop(int argc, char** argv)
{

	//opengl setup
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)screen_width / (float)screen_height, 0.1f, 100.0f);
	glm::mat4 view = camera.getViewMatrix();
	glm::mat4 model = glm::mat4(1.0f);

	glm::vec3 light_position(0.5f, 0.5f, -3.3f);

	glm::vec3 other_position;

	float* float_pointer;

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
	//plane
	BERender::Geometry plane_geometry;
	plane_geometry.makePlane();
	glm::mat4 plane_model = glm::scale(glm::mat4(1.0f),glm::vec3(10.0f,10.0f,10.0f));
	BERender::Material plane_material;
	plane_material.diffuse_color = glm::vec3(1.0f,1.0f,1.0f);
	plane_material.ambient_color = glm::vec3(0.0f,0.0f,0.0f);
	plane_material.specular_color = glm::vec3(1.0f,0.0f,0.0f);

	//other model
	BERender::Geometry sphere_geometry;
	sphere_geometry.makeIcoSphere();
	glm::mat4 sphere_model = glm::translate(glm::mat4(1.0),other_position);


	//networking setup
	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	struct addrinfo *result = NULL,
									*ptr = NULL,
									hints;
	const char *sendbuf = (const char*)&camera.position;
	char recvbuf[DEFAULT_BUFLEN];
	int iResult;
	int recvbuflen = DEFAULT_BUFLEN;
	// Validate the parameters
	if (argc != 2)
	{
		printf("usage: %s server-name\n", argv[0]);
		return;
	}

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		printf("WSAStartup failed with error: %d\n", iResult);
		return;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
	if (iResult != 0)
	{
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return;
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
	{

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
													 ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET)
		{
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return ;
		}

		// Connect to server.
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR)
		{
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}
	freeaddrinfo(result);
	if (ConnectSocket == INVALID_SOCKET)
	{
		printf("Unable to connect to server!\n");
		WSACleanup();
		return;
	}
	

	while (!glfwWindowShouldClose(window))
	{
		double current_frame = glfwGetTime();
		delta_time = current_frame - last_frame;
		last_frame = current_frame;
		processInput(window);

		//send/recieve
		iResult = send(ConnectSocket, sendbuf, sizeof(glm::vec3), 0);
		if (iResult == SOCKET_ERROR)
		{
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(ConnectSocket);
			WSACleanup();
			return;
		}
		iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0)
			printf("Bytes received: %d\n", iResult);
		else if (iResult == 0)
			printf("Connection closed\n");
		else
			printf("recv failed with error: %d\n", WSAGetLastError());
		
		float_pointer = (float*)recvbuf;
		other_position.x=*float_pointer;
		++float_pointer;
		other_position.y=*float_pointer;
		++float_pointer;
		other_position.z=*float_pointer;

		sphere_model = glm::translate(glm::mat4(1.0f),other_position);

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
		scene.add(new BERender::DrawableGeometry(&sphere_geometry,sphere_model,shader_program));
		scene.draw();
		text.RenderText(text_shader_program, "connecting to server", 25.0f, 25.0f, 1.0f, glm::vec3(1.0f,0.0f,0.0f));
		scene.clear();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	// shutdown the connection since no more data will be sent
	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR)
	{
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return;
	}
	closesocket(ConnectSocket);
  WSACleanup();
}