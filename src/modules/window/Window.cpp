#pragma once
#include "Window.h"


GLFWwindow* Window::getWindow()
{
	return this->window;
}

void Window::makeContextCurrent()
{
	glfwMakeContextCurrent(this->window);
}


Window::Window(unsigned int SCR_WIDTH, unsigned int SCR_HEIGHT, const char* title)
{
	this->SCR_HEIGHT = SCR_HEIGHT;
	this->SCR_WIDTH = SCR_WIDTH;
	this->title = title;
	this->init();
}

void Window::init()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	this->window = glfwCreateWindow(this->SCR_WIDTH, this->SCR_HEIGHT, this->title, NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}

}

