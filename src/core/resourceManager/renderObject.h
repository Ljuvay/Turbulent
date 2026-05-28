#ifndef RENDEROBJECT_H
#define RENDEROBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>

#include "mesh.h"
#include "shader.h"

struct renderObject
{
	Mesh* sourseMesh;
	Shader* sourceShader;
	//Material later

	glm::mat4 worldTransform;

	uint64_t objectID;
};

struct GLState
{
	bool wireframe = false;
	bool depthTest = true;
	bool depthMask = true;
	bool blend = false;
	bool cullFace = true;
};

#endif // !RENDEROBJECT_H
