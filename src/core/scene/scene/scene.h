#ifndef SCENE_H
#define SCENE_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

#include "window.h"

class forwardRenderer;
class sceneEditor;

class Camera;
class Shader;
class ResourceManager;

class Scene
{
public:
	Scene();
	virtual ~Scene();

	virtual void init() = 0;
	virtual void update(float dt, const Window& window) = 0;
	virtual void render(const Window& window) = 0;
	virtual void inputHandler(Window& window, float dt) = 0;
	virtual void onImGui() = 0;
	virtual void buildEditor(sceneEditor& editor) = 0;

	Camera& getCamera() { return *_Camera; }
	forwardRenderer& getRenderer() { return *m_Renderer; }
	ResourceManager& getResources() { return *m_RM; }
	void setRenderer(forwardRenderer& ForRend) { m_Renderer = &ForRend; }
	void setResources(ResourceManager& resMan) { m_RM = &resMan; }

	float fps = 0.0f;
	float frameTime = 0.0f;
	bool m_editorMode = false;

protected:
	std::unique_ptr<Camera> _Camera;
	ResourceManager* m_RM;
	forwardRenderer* m_Renderer;
};

#endif // !SCENE_H
