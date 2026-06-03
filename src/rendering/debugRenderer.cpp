#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>

#include "resourceManager.h"
#include "debugRenderer.h"

void debugRenderer::init(ResourceManager* rm)
{
	m_rm = rm;
	glGenVertexArrays(1, &m_gridVAO);
}

void debugRenderer::drawGrid(const glm::mat4& view, const glm::mat4& proj, const glm::vec3& camPos)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);

	uint32_t gridID = m_rm->shaders().shaderIDfromName("debugGrid");
	Shader& gridShader = m_rm->shaders().getShaderData(gridID);
	gridShader.use();
	gridShader.setMat4("view", view);
	gridShader.setMat4("projection", proj);
	gridShader.setVec3("camPos", camPos);

	glBindVertexArray(m_gridVAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}