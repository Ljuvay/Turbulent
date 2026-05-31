#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>

#include "forwardRenderer.h"

void forwardRenderer::init()
{
	renderQueue.reserve(1000); // Just do max 1000 for now
	sceneLights.reserve(16);
}

void forwardRenderer::setResources(ResourceManager* rm)
{
	resources = rm;
}

void forwardRenderer::beginFrame()
{
	renderQueue.clear();
	sceneLights.clear();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

void forwardRenderer::submitItem(const renderObject& renObj)
{
	renderQueue.push_back(renObj);
}

void forwardRenderer::submitLight(const lightSource& lightObj)
{
	sceneLights.push_back(lightObj);
}


// Need to eventually sort by GLState
void forwardRenderer::flush()
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

		shader->setVec3("ambient", obj.material.ambient);
		shader->setVec3("diffuse", obj.material.diffuse);
		shader->setVec3("specular", obj.material.specular);
		shader->setFloat("shininess", obj.material.shininess);

		shader->setInt("numLights", sceneLights.size());
		for (int i = 0; i < sceneLights.size(); i++)
		{
			std::string base = "lights[" + std::to_string(i) + "].";
			shader->setVec3(base + "position", sceneLights[i].position);
			shader->setVec3(base + "color", sceneLights[i].color);
			shader->setFloat(base + "strength", sceneLights[i].strength);
		}

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

void forwardRenderer::endFrame()
{
	// Leave empty for now
}

void forwardRenderer::setViewPos(const glm::vec3 viewPos)
{
	this->viewPos = viewPos;
}

void forwardRenderer::setViewProj(const glm::mat4& view, const glm::mat4 projection)
{
	this->view = view;
	this->projection = projection;
}
