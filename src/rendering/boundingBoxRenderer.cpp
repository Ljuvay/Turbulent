#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>
#include <iostream>

#include "boundingBoxRenderer.h"

BoundingBoxRenderer::BoundingBoxRenderer()
{
	glGenVertexArrays(1, &_VAO);
	glBindVertexArray(_VAO);

	glGenBuffers(1, &_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW); // Empty buffer for placeholder

	glGenBuffers(1, &_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW); // Empty buffer for palceholder

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(0); // Vertices

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Initialize counters
	_indexCount = 0;
	_VBOsize = 0;
	_EBOsize = 0;
}

BoundingBoxRenderer::~BoundingBoxRenderer()
{
	glDeleteVertexArrays(1, &_VAO);
	glDeleteBuffers(1, &_VBO);
	glDeleteBuffers(1, &_EBO);
}

// If the size of the VBO/EBO has grown we need to re-initialize the buffer, but if just the data has changed we can just swap it
void BoundingBoxRenderer::updateGPU(const std::vector<glm::vec3>& vertices, const std::vector<GLuint>& indices) {
	GLuint newSizeV = sizeof(glm::vec3) * vertices.size();
	GLuint newSizeI = sizeof(unsigned int) * indices.size();

	glBindVertexArray(_VAO);

	// Update VBO
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	if (newSizeV != _VBOsize) {
		glBufferData(GL_ARRAY_BUFFER, newSizeV, vertices.data(), GL_DYNAMIC_DRAW);
		_VBOsize = newSizeV;
	}
	else if (newSizeV > 0)
	{
		glBufferSubData(GL_ARRAY_BUFFER, 0, newSizeV, vertices.data());
	}

	// Update EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
	if (newSizeI != _EBOsize) {
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, newSizeI, indices.data(), GL_DYNAMIC_DRAW);
		_EBOsize = newSizeI;
	}
	else if (newSizeI > 0)
	{
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, newSizeI, indices.data());
	}

	glBindVertexArray(0);

	_indexCount = static_cast<GLsizei>(indices.size());
}

 void BoundingBoxRenderer::drawCube() const
{
	glBindVertexArray(_VAO);
	glDrawElements(GL_TRIANGLES, _indexCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}