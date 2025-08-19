#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>


class Window
{
private:
	GLFWwindow* window;
	unsigned int SCR_WIDTH,
		SCR_HEIGHT;
	const char* title;
	void init();
	
public:
	Window(unsigned int SCR_WIDTH = 900, unsigned int SCR_HEIGHT = 600, const char* title = "");
	void makeContextCurrent();
	GLFWwindow* getWindow();
};