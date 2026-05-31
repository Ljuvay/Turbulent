#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>
#include "camera.h"
class Scene;

class Window {
public:
	Window(int w, int h);
	~Window();

	GLFWwindow* getWindow();

	float getWidth() const;
	float getHeight() const;

	Scene* _scene = nullptr;
	void setScene(Scene* scene) { _scene = scene; }

	void beginFrame();
	void endFrame();

	bool editorMode = false;
private:
	int _SCR_WIDTH;
	int _SCR_HEIGHT;

	GLFWwindow* _window;

	static void framebufferCallback(GLFWwindow* window, int w, int h);
	static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
};

#endif // !WINDOW_H