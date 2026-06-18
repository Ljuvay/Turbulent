#ifndef SCENEOBJECTS_H
#define SCENEOBJECTS_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <memory>
#include <vector>

#include "mesh.h"
#include "vertex.h"
#include "renderObject.h"

struct instancedModel
{
	std::string name = "";
	renderObject obj;

};

struct instancedLight
{
	std::string name = "";
	lightSource lt;

};

class sceneObjects
{
public:
	void addModel();
	void addLight();

	void renderObjects();
	void renderLights();

private:
	std::vector<instancedModel> m_sceneLights;
	std::vector<instancedModel> m_sceneModels;
};

#endif // !SCENEOBJECTS_H
