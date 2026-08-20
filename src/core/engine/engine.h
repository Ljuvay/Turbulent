#ifndef ENGINE_H
#define ENGINE_H

#include <memory>
#include <string>

#include "forwardRenderer.h"
#include "sceneManager.h"
#include "resourceManager.h"
#include "window.h"
#include "input.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

struct EngineContext
{
	EngineContext(const int& windowWidth, const int& windowHeight, std::string windowName)
		: m_window(windowWidth, windowHeight, windowName) 
	{}

	Window m_window;
	std::unique_ptr<forwardRenderer> m_Renderer;
	sceneManager m_sceneManager;
	ResourceManager m_resourceManager;
	bool m_editorMode = false;
};

class Engine
{
public:
	Engine();
	void init();
	void run();
	void shutdown();
private:
	EngineContext m_EngineContext;
};

#endif // !ENGINE_H
