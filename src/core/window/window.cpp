#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "window.h"
#include "scene.h"

Window::Window(int w, int h)
	: _SCR_WIDTH(w), _SCR_HEIGHT(h), _window(nullptr)
{

	// Initlaize GLFW
	if (!glfwInit()) {
		std::cerr << "FAILED TO INITIALIZE GLFW\n";
		return;
	}

	// Setup OpenGL context
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create GLFW Window
	_window = glfwCreateWindow(_SCR_WIDTH, _SCR_HEIGHT, "TEngine 2026", nullptr, nullptr);
	if(!_window)
	{
		std::cerr << "FAILED TO CREATE GLFW WINDOW\n";
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(_window);

	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "FAILED TO INITIALIZE GLAD\n";
		return;
	}

	glViewport(0, 0, _SCR_WIDTH, _SCR_HEIGHT);
	glfwSetWindowUserPointer(_window, this);
	glfwSetFramebufferSizeCallback(_window, framebufferCallback);
	glfwSetCursorPosCallback(_window, mouseCallback);
	glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_PROGRAM_POINT_SIZE); 
}

Window::~Window()
{
	if (_window)
	{
		glfwSetWindowUserPointer(_window, nullptr);
		glfwDestroyWindow(_window);
		_window = nullptr;
	}
	glfwTerminate();
}

GLFWwindow* Window::getWindow()
{
	return _window;
}

float Window::getWidth() const
{
	return _SCR_WIDTH;
}

float Window::getHeight() const
{
	return _SCR_HEIGHT;
}

void Window::beginFrame()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::endFrame()
{
	glfwSwapBuffers(this->getWindow());
	glfwPollEvents();
}

void Window::mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	if (!window) return;

	Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
	if (!win) { return; }

	static float lastX;
	static float lastY;
	static int skipFrame = 2;

	if (skipFrame-- > 0) {
		lastX = (float)xpos;
		lastY = (float)ypos;
		return;
	}

	float xoffset = (float)xpos - lastX;
	float yoffset = lastY - (float)ypos; // reversed

	lastX = (float)xpos;
	lastY = (float)ypos;

	if (win->editorMode) return;
	win->_scene->getCamera().ProcessMouseMovement(xoffset, yoffset);
}

void Window::framebufferCallback(GLFWwindow* window, int width, int height)
{
	if (!window) return;

	Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
	if (!win) { return; }

	win->_SCR_WIDTH = (float)width;
	win->_SCR_HEIGHT = (float)height;
	glViewport(0, 0, width, height);
}