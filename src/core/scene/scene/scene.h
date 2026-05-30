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
	Camera& getCamera() {
		return *_Camera;
	}

protected:
	std::unique_ptr<forwardRenderer> _NRenderer;
	std::unique_ptr<Camera> _Camera;
	std::shared_ptr<ResourceManager> _RM;

	bool pressedLastFrame;
};

#endif // !SCENE_H
