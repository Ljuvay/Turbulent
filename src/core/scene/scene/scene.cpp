#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <iostream>
#include <string>

#include "scene.h"
#include "shader.h"
#include "camera.h"
#include "window.h"
#include "forwardRenderer.h"
#include "resourceManager.h"
#include "mesh.h"
#include "vertex.h"

Scene::Scene()
	:
	_NRenderer(std::make_unique<forwardRenderer>()),
	_Camera(std::make_unique<Camera>()),
	_RM(std::make_shared<ResourceManager>())
{
	pressedLastFrame = false;
}

Scene::~Scene(){}

void Scene::update(float dt, const Window& window) {}

void Scene::render(const Window& window) {}

void Scene::inputHandler(Window& window, float dt) {}