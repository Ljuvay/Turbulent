#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>

#include "newRenderer.h"

void newRenderer::init()
{
	renderQueue.reserve(1000); // Just do max 1000 for now
}

void newRenderer::setResources(ResourceManager* rm)
{
	resources = rm;
}

void newRenderer::beginFrame()
{
	renderQueue.clear();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

void newRenderer::submitItem(const renderObject& renObj)
{
	renderQueue.push_back(renObj);
}

// Need to eventually sort by GLState
void newRenderer::flush()
{
	for (const auto& obj : renderQueue)
	{
		obj.state.depthTest ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
		obj.state.cullFace ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
		obj.state.blend ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
		obj.state.wireframe ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDepthMask(obj.state.depthMask ? GL_TRUE : GL_FALSE);

		Shader* shader = &resources->shaders().getShaderData(obj.shaderID);
		gpuMesh* mesh = &resources->meshes().getGPUMesh(obj.meshID);
		//Material* mat = resourceManager.
		
		shader->use();
		shader->setVec3("viewPos", viewPos);
		shader->setMat4("model", obj.worldTransform);
		shader->setMat4("view", view);
		shader->setMat4("projection", projection);
		// Set material uniforms
		glBindVertexArray(mesh->VAO);

		glDrawElements(
			GL_TRIANGLES,
			mesh->indexCount,
			GL_UNSIGNED_INT,
			nullptr
		);
	}
}

void newRenderer::endFrame()
{
	// Leave empty for now
}

void newRenderer::setViewPos(const glm::vec3 viewPos)
{
	this->viewPos = viewPos;
}

void newRenderer::setViewProj(const glm::mat4& view, const glm::mat4 projection)
{
	this->view = view;
	this->projection = projection;
}
