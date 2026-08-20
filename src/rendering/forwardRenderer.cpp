#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "forwardRenderer.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

void forwardRenderer::init()
{
	renderQueue.reserve(1000); // Just do max 1000 for now
	sceneLights.reserve(8);

	m_shadowFBO.create(1024, 1024, true);
}

void forwardRenderer::setResources(ResourceManager* rm)
{
	resources = rm;
}

void forwardRenderer::beginFrame()
{
	renderQueue.clear();
	sceneLights.clear();

	glClearColor(clearColor.x, clearColor.y, clearColor.z, 1.0f);

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
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
void forwardRenderer::flush(int scr_width, int scr_height)
{
	int shadowLightIdx = -1;
	for (int i = 0; i < sceneLights.size(); i++)
	{
		if (sceneLights[i].castsShadows) { shadowLightIdx = i; break; }
	}

	if (shadowLightIdx >= 0) {
		glm::mat4 lsm = sceneLights[shadowLightIdx].getLightSpaceMatrix(glm::vec3(0.0f), 200.0f);
		this->lightSpaceMatrix = lsm;
		renderShadowPass(lightSpaceMatrix);
	}

	glViewport(0, 0, scr_width, scr_height);

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
		shader->setMat4("model", obj.worldTransform.getWorldTransform());
		shader->setMat4("view", view);
		shader->setMat4("projection", projection);
		shader->setMat4("lightSpaceMatrix", lightSpaceMatrix);

		glActiveTexture(GL_TEXTURE0 + 7);
		glBindTexture(GL_TEXTURE_2D, m_shadowFBO.getDepthTexture());
		shader->setInt("shadowMap", 7);
		shader->setInt("shadowLightIndex", shadowLightIdx);

		shader->setVec3("ambient", obj.material.ambient);
		shader->setVec3("diffuse", obj.material.diffuse);
		shader->setVec3("specular", obj.material.specular);
		shader->setFloat("shininess", obj.material.shininess);

		shader->setBool("hasTexture", obj.hasTexture);
		if (obj.hasTexture)
		{
			for (int t = 0; t < obj.textureCount; t++) {
				glActiveTexture(GL_TEXTURE0 + t);
				GLuint glTex = resources->textures().getGLTextureID(obj.textureIDs[t]);
				glBindTexture(GL_TEXTURE_2D, glTex);
				shader->setInt("textures[" + std::to_string(t) + "]", t);
				obj.useScaleTiling ? 
					shader->setVec2("tiling", glm::vec2(obj.worldTransform.scale.x * obj.tiling.x, obj.worldTransform.scale.z * obj.tiling.y)) :
					shader->setVec2("tiling", obj.tiling);
			}
		}

		shader->setInt("numLights", sceneLights.size());
		for (int i = 0; i < sceneLights.size(); i++)
		{
			std::string base = "lights[" + std::to_string(i) + "].";
			shader->setVec3(base + "position", sceneLights[i].position);
			shader->setVec3(base + "direction", sceneLights[i].direction);
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

	glDisable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);
	for (int i = 0; i < sceneLights.size(); i++)
	{
		if (sceneLights[i].draw) {
			uint32_t bbID = resources->meshes().meshIDfromName("billboard");
			gpuMesh* mesh = &resources->meshes().getGPUMesh(bbID);
			uint32_t sID = resources->shaders().shaderIDfromName("billboardParticle");
			Shader* shader = &resources->shaders().getShaderData(sID);

			shader->use();
			shader->setVec3("viewPos", viewPos);
			shader->setMat4("view", view);
			shader->setMat4("projection", projection);
			shader->setVec3("Pposition", sceneLights[i].position);
			shader->setVec3("color", sceneLights[i].color);

			glBindVertexArray(mesh->VAO);

			glDrawElements(
				GL_TRIANGLES,
				mesh->indexCount,
				GL_UNSIGNED_INT,
				nullptr
			);
		}
	}
}

void forwardRenderer::endFrame()
{
	// Leave empty for now
}

void forwardRenderer::setViewPos(const glm::vec3& viewPos)
{
	this->viewPos = viewPos;
}

void forwardRenderer::setViewProj(const glm::mat4& view, const glm::mat4& projection)
{
	this->view = view;
	this->projection = projection;
}

void forwardRenderer::setLightSpaceMatrix(const glm::mat4& matrix)
{
	this->lightSpaceMatrix = matrix;
}

void forwardRenderer::renderShadowPass(const glm::mat4& lightSpaceMatrix)
{
	//Calc light matrix
	m_shadowFBO.bind();
	glViewport(0, 0, m_shadowFBO.getWidth(), m_shadowFBO.getHeight());
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);

	Shader* shader = &resources->shaders().getShaderData(resources->shaders().shaderIDfromName("depthShadow"));
	shader->use();
	shader->setMat4("lightSpaceMatrix", lightSpaceMatrix);

	ImGui::Begin("Shadow Map Debug");
	ImGui::Image(
		(ImTextureID)(intptr_t)m_shadowFBO.getDepthTexture(),
		ImVec2(256, 256),
		ImVec2(0, 1),
		ImVec2(1, 0)
	);
	ImGui::End();

	for (renderObject& o : renderQueue)
	{
		gpuMesh* mesh = &resources->meshes().getGPUMesh(o.meshID);
		shader->setMat4("model", o.worldTransform.getWorldTransform());
		glBindVertexArray(mesh->VAO);
		glDrawElements(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_INT, nullptr);
	}

	m_shadowFBO.unbind();
	//glViewport(0, 0, 800, 600); // 800 600 needs to be changed to the resized windows dimensions
}

void forwardRenderer::drawMesh()
{

}

void forwardRenderer::drawPoint(const glm::vec3& position, const glm::vec3& color)
{

	
}