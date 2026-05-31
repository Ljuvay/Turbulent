#ifndef RENDEROBJECT_H
#define RENDEROBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>

#include "mesh.h"
#include "shader.h"

struct GLState
{
	bool wireframe = false;
	bool depthTest = true;
	bool depthMask = true;
	bool blend = false;
	bool cullFace = true;
};

struct lightSource
{
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 color = glm::vec3(0.0f);
	float strength = 0.1f;
};

struct Material
{
	glm::vec3 ambient = glm::vec3(0.0f);
	glm::vec3 diffuse = glm::vec3(0.0f);
	glm::vec3 specular = glm::vec3(0.0f);
	float shininess = 0.0f;
};

struct renderObject
{
	uint32_t meshID = 0;
	uint32_t shaderID = 0;
	Material material;
	glm::mat4 worldTransform = glm::mat4(1.0f);
	GLState state;
};

#endif // !RENDEROBJECT_H
