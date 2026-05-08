#include <vector>
#include <iostream>

#include "scene.h"
#include "sceneManager.h"

void sceneManager::initScene()
{
	Scene* gameplayScene = new Scene();
}

void sceneManager::setScene(Scene scenetoSet)
{
	activeScene = g_scenes["gameplayScene"].get();
	std::cout << "Switched to Gameplay scene" << std::endl;
}