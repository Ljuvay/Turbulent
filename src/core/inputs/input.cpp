#include <GLFW/glfw3.h>

#include <vector>

#include "input.h"

namespace Input
{
	GLFWwindow* window;

	bool keyPressed[372];
	bool keyDown[372];
	bool keyDownLastFrame[372];

	void init(void* glfwWindow)
	{
		window = static_cast<GLFWwindow*>(glfwWindow);
	}

	void Update()
	{
		for (int i = 32; i < 349; i++)
		{
			if (glfwGetKey(window, i) == GLFW_PRESS)
			{
				keyDown[i] = true;
			}
			else
			{
				keyDown[i] = false;
			}

			if (keyDown[i] && !keyDownLastFrame[i])
			{
				keyPressed[i] = true;
			}
			else
			{
				keyPressed[i] = false;
			}
			keyDownLastFrame[i] = keyDown[i];
		}
	}

	void clearKeyStates()
	{
		for (int i = 32; i < 349; i++)
		{
			keyDown[i] = false;
			keyPressed[i] = false;
			keyDownLastFrame[i] = false;
		}
	}

	bool KeyPressed(unsigned int keyCode)
	{
		return keyPressed[keyCode];
	}

	bool KeyDown(unsigned int keyCode)
	{
		return keyDown[keyCode];
	}
}