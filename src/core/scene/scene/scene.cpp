#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <iostream>
#include <string>

#include "scene.h"
#include "boundingBoxRenderer.h"
#include "particleRenderer.h"
#include "meshRenderer.h"
#include "shader.h"
#include "camera.h"
#include "window.h"
#include "newRenderer.h"
#include "resourceManager.h"
#include "mesh.h"
#include "vertex.h"

Scene::Scene()
	:
	_NRenderer(std::make_unique<newRenderer>()),
	_BBRenderer(std::make_unique<BoundingBoxRenderer>()),
	_MRenderer(std::make_unique<meshRenderer>()),
	_PRenderer(std::make_unique<particleRenderer>()),
	_Camera(std::make_unique<Camera>()),
	_RM(std::make_shared<ResourceManager>())
{
	pressedLastFrame = false;
}

Scene::~Scene(){}

void Scene::update(float dt, const Window& window) {}

void Scene::render(const Window& window) {}

void Scene::inputHandler(Window& window, float dt) {}