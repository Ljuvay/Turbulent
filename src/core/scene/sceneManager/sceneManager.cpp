#include <vector>
#include <iostream>
#include <string>

#include "scene.h"
#include "sceneManager.h"
#include "defaultScene.h"

void sceneManager::initDefaultScene()
{
	g_scenes["default"] = std::make_unique<defaultScene>();
	activeScene = g_scenes["default"].get();
	activeScene->init();
}

void sceneManager::setScene(std::string sceneName)
{
	activeScene = g_scenes[sceneName].get();
	std::cout << "Switched to " << sceneName << std::endl;
}

void sceneManager::updateScene(float dt, const Window& window)
{
	activeScene->update(dt, window);
}

void sceneManager::renderScene(const Window& window)
{
	activeScene->render(window);
}

Scene* sceneManager::getScene()
{
	return activeScene;
}

