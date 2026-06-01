#ifndef RENDEROBJECT_H
#define RENDEROBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
	bool useAmbient = true;
};

struct Transform
{
	glm::vec3 translation = glm::vec3(0.0f);
	glm::vec3 rotation = glm::vec3(0.0f);
	glm::vec3 scale = glm::vec3(1.0f);
	glm::mat4 getWorldTransform() const
	{
		glm::mat4 worldTransform = glm::mat4(1.0f);

		worldTransform = glm::translate(glm::mat4(1.0f), translation);

		worldTransform *= glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1,0,0));
		worldTransform *= glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(0,1,0));
		worldTransform *= glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), glm::vec3(0,0,1));

		worldTransform *= glm::scale(glm::mat4(1.0f), scale);
		return worldTransform;
	}
};

struct renderObject
{
	uint32_t meshID = 0;
	uint32_t shaderID = 0;
	Material material;
	Transform worldTransform;
	GLState state;
};

#endif // !RENDEROBJECT_H
