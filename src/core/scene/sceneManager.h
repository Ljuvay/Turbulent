#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <vector>
#include <iostream>
#include <>

#include "scene.h"

class sceneManager
{
public:
	void initScene();

	void setScene(Scene scenetoSet);

private:
	std::unordered_map<std::string, std::unique_ptr<Scene>> g_scenes;
	Scene* activeScene = nullptr;
};


#endif // !SCENEMANAGER_H