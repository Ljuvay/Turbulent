#ifndef ENGINE_H
#define ENGINE_H

#include <memory>

#include "forwardRenderer.h"
#include "sceneManager.h"
#include "resourceManager.h"
#include "window.h"
#include "input.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class Engine
{
public:
	Engine();
	void init();
	void run();
	void shutdown();
private:
	Window m_window;
	std::unique_ptr<forwardRenderer> m_Renderer;
	sceneManager m_sceneManager;
	ResourceManager m_resourceManager;
};

#endif // !ENGINE_H
