#include "ParticlePhysicsTest1App.h"
using namespace BEEngine;
void ParticlePhysicsTest1App::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void ParticlePhysicsTest1App::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
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

void ParticlePhysicsTest1App::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.processMouseScroll((float)yoffset);
}

void ParticlePhysicsTest1App::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		BEPhysics::Particle particle;
		particle.position = camera.position + camera.front;
		particle.velocity = speed*camera.front;
		particle.inverse_mass = 1. / 10.;
		particle.damping = 0.9;
		particle.acceleration = BEPhysics::Vector3(0,-9.8,0);
		particles.push_back(particle);
	}
}

void ParticlePhysicsTest1App::processInput(GLFWwindow* window)
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


void ParticlePhysicsTest1App::initialize()
{
	screen_width = 800;
	screen_height = 600;
	delta_time = 0.0;
	last_frame = 0.0;
	last_x = screen_width / 2;
	last_y = screen_height / 2;
	first_mouse = true;
	camera = BERender::Camera(glm::vec3(0.0, 2.0, 3.0));
	speed = 10;
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

void ParticlePhysicsTest1App::mainLoop()
{

	contact_resolver = BEPhysics::ParticleContactResolver(10);
	contact = new BEPhysics::ParticleContact[10];
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
	BERender::Geometry sphere_geometry;
	glm::mat4 sphere_model = glm::scale(glm::mat4(1.f),glm::vec3(0.1f));
	sphere_geometry.makeIcoSphere();

	//physics:
	BEPhysics::Particle particle;
	particle.position = BEPhysics::Vector3(-1, 2.5, 0);
	particle.velocity = BEPhysics::Vector3(0);
	particle.inverse_mass = 1. / 65.;
	particle.damping = 0.98;
	particle.acceleration = BEPhysics::Vector3(0);
	particles.push_back(particle);
	particle.position = BEPhysics::Vector3(-1, 2.2, 0);
	particles.push_back(particle);
	BEPhysics::ParticleRodConstraint rod1 = BEPhysics::ParticleRodConstraint();
	rod1.particle = &particles[0];
	rod1.length = glm::length(particles[0].position - BEPhysics::Vector3(0, 3, 0));
	rod1.anchor = BEPhysics::Vector3(0, 3, 0);
	BEPhysics::ParticleRodConstraint rod2 = BEPhysics::ParticleRodConstraint();
	rod2.particle = &particles[1];
	rod2.length = glm::length(particles[1].position - BEPhysics::Vector3(0, 3, 0));
	rod2.anchor = BEPhysics::Vector3(0, 3, 0);
	BEPhysics::ParticleGravity gravity(BEPhysics::Vector3(0, -9.8, 0));
	contact_generator.push_back(&rod1);
	contact_generator.push_back(&rod2);
	force_generator.push_back(BEPhysics::ParticleForceRegistration(&particles[0], &gravity));
	force_generator.push_back(BEPhysics::ParticleForceRegistration(&particles[1], &gravity));

	//line
	glm::vec3 positions[] = {particles[0].position, rod1.anchor, particles[1].position, rod2.anchor };
	unsigned int indices[] = {0, 1, 2, 3};
	unsigned int VAO, EBO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec3), &positions[0], GL_DYNAMIC_DRAW);
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
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
		unsigned int contacts_left = contact_resolver.iterations;
		for (unsigned int i = 0; i < contact_generator.size(); i++)
		{
			contacts_left -= contact_generator[i]->addContact(contact+(contact_resolver.iterations-contacts_left), contacts_left);
		}
		contact_resolver.resolveContacts(contact, contact_resolver.iterations - contacts_left, delta_time);
		for (unsigned int i = 0; i < force_generator.size(); i++)
		{
			force_generator[i].fg->applyForce(force_generator[i].particle, delta_time);
		}
		for (unsigned int i = 0; i < particles.size(); i++)
		{
			glm::mat4 m = glm::translate(glm::mat4(1.f), glm::vec3(particles[i].position));
			m = m * sphere_model;
			particles[i].integrate(delta_time);
			scene.add(new BERender::DrawableGeometry(&sphere_geometry,
				m, 
				shader_program));
		}
		scene.add(new BERender::DrawableGeometry(&sphere_geometry,
			glm::translate(glm::mat4(1.f), glm::vec3(rod1.anchor)) * sphere_model,
			shader_program));

		scene.draw();
		scene.clear();

		//rendering the line
		positions[0] = particles[0].position;
		positions[2 * 1] = particles[1].position;
		glBindVertexArray(VAO);
		glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec3), &positions[0], GL_DYNAMIC_DRAW);
		line_shader.use();
		line_shader.setMat4("model", model);
		line_shader.setMat4("view", view);
		line_shader.setMat4("projection", projection);
		glDrawElements(GL_LINES, 4, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
}

void ParticlePhysicsTest1App::run()
{
	initialize();
	mainLoop();
}