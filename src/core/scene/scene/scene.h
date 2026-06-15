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
	Camera& getCamera() { return *_Camera; }
	void setRenderer(forwardRenderer& ForRend) { m_Renderer = &ForRend; }
	void setResources(ResourceManager& resMan) { m_RM = &resMan; }

	float fps = 0.0f;
	float frameTime = 0.0f;

protected:
	std::unique_ptr<Camera> _Camera;
	ResourceManager* m_RM;
	forwardRenderer* m_Renderer;
};

#endif // !SCENE_H
