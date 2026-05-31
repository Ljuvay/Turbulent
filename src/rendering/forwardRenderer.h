#ifndef FORWARDRENDERER_H
#define FORWARDRENDERER_H

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <vector>

#include "resourceManager.h"
#include "renderObject.h"
#include "mesh.h"
#include "vertex.h"

class forwardRenderer
{
public:
	void init();
	void setResources(ResourceManager* rm);

	void beginFrame();
	void submitItem(const renderObject& renObj);
	void submitLight(const lightSource& lightObj);
	void flush();
	void endFrame();

	void setViewPos(const glm::vec3 viewPos);
	void setViewProj(const glm::mat4& view, const glm::mat4 proj);

private:
	ResourceManager* resources = nullptr;

	std::vector<renderObject> renderQueue;
	std::vector<lightSource> sceneLights;

	glm::vec3 viewPos;
	glm::mat4 view = glm::mat4(0.0f);
	glm::mat4 projection = glm::mat4(0.0f);
};

#endif // !FORWARDRENDERER_H