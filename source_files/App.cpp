#include "App.h"
using namespace BEEngine;

void BEEngine::framebuffer_size_callback_friend(GLFWwindow* window, int width, int height)
{
	App* app = (App*)glfwGetWindowUserPointer(window);
	if (app != NULL)
	{
		app->framebuffer_size_callback(window, width, height);
	}
}

void BEEngine::mouse_callback_friend(GLFWwindow* window, double xposIn, double yposIn)
{
	App* app = (App*)glfwGetWindowUserPointer(window);
	if (app != NULL)
	{
		app->mouse_callback(window, xposIn, yposIn);
	}
}

void BEEngine::scroll_callback_friend(GLFWwindow* window, double xoffset, double yoffset)
{
	App* app = (App*)glfwGetWindowUserPointer(window);
	if (app != NULL)
	{
		app->scroll_callback(window, xoffset, yoffset);
	}
}

void BEEngine::mouse_button_callback_friend(GLFWwindow* window, int button, int action, int mods)
{
	App* app = (App*)glfwGetWindowUserPointer(window);
	if (app != NULL)
	{
		app->mouse_button_callback(window, button, action, mods);
	}
}