#include "ForcesTest.h"
using namespace BEEngine;
void ForcesTest::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void ForcesTest::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
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

void ForcesTest::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.processMouseScroll((float)yoffset);
}

void ForcesTest::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
	}
}

void ForcesTest::processInput(GLFWwindow* window)
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


void ForcesTest::initialize()
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
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetMouseButtonCallback(window, mouse_button_callback_friend);
}

void ForcesTest::mainLoop()
{

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_LINE_SMOOTH);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)screen_width / (float)screen_height, 0.1f, 100.0f);
	glm::mat4 view = camera.getViewMatrix();
	glm::mat4 model = glm::mat4(1.0f);

	glm::vec3 light_position(0.5f, 0.5f, -3.3f);

	BERender::Shader shader_program("Shaders/VertexShader7.glsl", "Shaders/FragmentShader10.glsl");
	BERender::Shader line_shader("Shaders/VertexShader8.glsl", "Shaders/FragmentShader11.glsl");
	shader_program.use();
	shader_program.setVec3("point_light.position", light_position);
	shader_program.setVec3("point_light.ambient_color", 0.5f, 0.5f, 0.5f);
	shader_program.setVec3("point_light.diffuse_color", 0.8f, 0.8f, 0.8f);
	shader_program.setVec3("point_light.specular_color", 1.0f, 1.0f, 1.0f);
	shader_program.setFloat("point_lights.constant", 1.0f);
	shader_program.setFloat("point_lights.linear", 0.09f);
	shader_program.setFloat("point_lights.quadratic", 0.032f);

	//rendering
	BERender::Scene scene;
	BERender::Geometry plane_geometry;
	plane_geometry.makePlane();
	BERender::Geometry box_geometry;
	glm::mat4 sphere_model = glm::scale(glm::mat4(1.f), glm::vec3(0.1f));
	box_geometry.makeCube();

	//physics:
	BEPhysics::RigidBody body;
	body.inverse_mass = 1. / 50.;
	body.inverse_inertia_tensor = BEPhysics::Matrix3(1.);
	body.linear_damping = 0.99;
	body.angular_damping = 0.99;
	body.can_sleep = false;
	body.position = BEPhysics::Vector3(-3, 2.5, 0);
	body.orientation = glm::rotate(BEPhysics::Quaternion(1, 0, 0, 0),
		(BEPhysics::real)glm::radians(0.), (BEPhysics::Vector3(1, 0, 0)));
	body.angular_velocity = BEPhysics::Vector3(0., 0., 0.);
	body.setAwake();
	body.acceleration = BEPhysics::Vector3(0);

	body.velocity = BEPhysics::Vector3(0, 0., 0);
	bodies.push_back(body);
	body.position = BEPhysics::Vector3(0, 4, 0);
	body.calculateDerivedData();
	BEPhysics::Gravity gravity(BEPhysics::Vector3(0, -9.8, 0));
	force_generators.push_back(BEPhysics::ForceRegistration(&bodies[0], &gravity));
	BEPhysics::Spring spring(BEPhysics::Vector3(.5, 0.5, 0.5), &body, BEPhysics::Vector3(0, 0, 0), 10, 1.5);
	force_generators.push_back(BEPhysics::ForceRegistration(&bodies[0], &spring));
	BEPhysics::Buoyancy water(BEPhysics::Vector3(0,-0.,0),
		(BEPhysics::real)0.5, 1., 0.);
	force_generators.push_back(BEPhysics::ForceRegistration(&bodies[0], &water));
	//rendering
		//line
	glm::vec3 positions[] = { body.position, bodies[0].getPointInWorldSpace(spring.connection_point)};
	unsigned int indices[] = { 0, 1};
	unsigned int VAO, EBO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(glm::vec3), &positions[0], GL_DYNAMIC_DRAW);
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
	glBindVertexArray(0);



	last_frame = glfwGetTime();
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
		for (unsigned int i = 0; i < force_generators.size(); i++)
		{
			force_generators[i].fg->updateForce(force_generators[i].body, delta_time);
		}
		for (unsigned int i = 0; i < bodies.size(); i++)
		{
			bodies[i].integrate(delta_time);
		}
		scene.add(new BERender::DrawableGeometry(&box_geometry, bodies[0].transform_matrix, shader_program));
		scene.add(new BERender::DrawableGeometry(&box_geometry, body.transform_matrix, shader_program));

		scene.draw();
		scene.clear();
		//rendering the line
		positions[0] = body.position;
		positions[1] = bodies[0].getPointInWorldSpace(spring.connection_point);
		glBindVertexArray(VAO);
		glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(glm::vec3), &positions[0], GL_DYNAMIC_DRAW);
		line_shader.use();
		line_shader.setMat4("model", model);
		line_shader.setMat4("view", view);
		line_shader.setMat4("projection", projection);
		glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
}

void ForcesTest::run()
{
	initialize();
	mainLoop();
}