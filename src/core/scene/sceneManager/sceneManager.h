#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <vector>
#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>

#include "scene.h"
#include "sceneEditor.h"

class forwardRenderer;
class ResourceManager;

class sceneManager
{
public:
	void initDefaultScene();

	void setScene(std::string sceneName);

	void updateScene(float dt, const Window& window);
	void renderScene(const Window& window);

	Scene* getScene();

private:
	std::unordered_map<std::string, std::unique_ptr<Scene>> g_scenes;
	std::unique_ptr<sceneEditor> m_editor;
	Scene* activeScene = nullptr;
};


#endif // !SCENEMANAGER_H