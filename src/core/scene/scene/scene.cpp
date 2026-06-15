#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <iostream>
#include <string>

#include "scene.h"
#include "camera.h"
#include "window.h"

Scene::Scene()
	:
	_Camera(std::make_unique<Camera>())
{

}

Scene::~Scene(){}

void Scene::update(float dt, const Window& window) {}

void Scene::render(const Window& window) {}

void Scene::inputHandler(Window& window, float dt) {}

void Scene::onImGui() {}
