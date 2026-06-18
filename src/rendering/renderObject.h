#ifndef RENDEROBJECT_H
#define RENDEROBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "mesh.h"
#include "shader.h"
#include "transform.h"

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
	bool draw = false;
};

struct Material
{
	glm::vec3 ambient = glm::vec3(0.0f);
	glm::vec3 diffuse = glm::vec3(0.0f);
	glm::vec3 specular = glm::vec3(0.0f);
	float shininess = 0.0f;
	bool useAmbient = true;
};

struct renderObject
{
	uint32_t meshID = 0;
	uint32_t shaderID = 0;
	uint32_t textureIDs[4] = {0, 0, 0, 0};

	int textureCount = 0;
	glm::vec2 tiling = glm::vec2(0.1f);

	Material material;
	Transform worldTransform;
	GLState state;

	bool useScaleTiling = false;
	bool hasTexture = false;
};

#endif // !RENDEROBJECT_H
