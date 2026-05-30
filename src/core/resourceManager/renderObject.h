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

// GET THIS OUT OF HERE ASAP
struct Material
{
	// Stuff like metallic, roughness, etc
	// Take a look at blender to help
	glm::vec3 albedo = glm::vec3(0.0f);
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
