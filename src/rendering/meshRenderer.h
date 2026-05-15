#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>
#include <iostream>

#include "mesh.h"
#include "vertex.h"

class meshRenderer {
public:
	meshRenderer()
	{
		glGenVertexArrays(1, &_VAO);
		glBindVertexArray(_VAO);

		glGenBuffers(1, &_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, _VBO);
		glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW); // Empty buffer for placeholder

		glGenBuffers(1, &_EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW); // Empty buffer for palceholder

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
		glEnableVertexAttribArray(2);

		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		glEnableVertexAttribArray(3);

		glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, radius));
		glEnableVertexAttribArray(4);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		// Initialize counters
		_indexCount = 0;
		_vertexCount = 0;
		_VBOsize = 0;
		_EBOsize = 0;
	}

	~meshRenderer()
	{
		glDeleteVertexArrays(1, &_VAO);
		glDeleteBuffers(1, &_VBO);
		glDeleteBuffers(1, &_EBO);
	}

	void updateGPU(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices)
	{
		GLuint newSizeV = sizeof(Vertex) * vertices.size();
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

		_vertexCount = static_cast<GLsizei>(vertices.size());
		_indexCount = static_cast<GLsizei>(indices.size());
	}

	void drawMesh() const
	{
		glBindVertexArray(_VAO);
		glDrawElements(GL_TRIANGLES, _indexCount, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void drawLines() const
	{
		glBindVertexArray(_VAO);
		glDrawArrays(GL_LINES, 0, _vertexCount);
		glBindVertexArray(0);
	}

private:
	GLuint _VAO;
	GLuint _VBO;
	GLuint _EBO;
	GLuint _indexCount;
	GLuint _vertexCount;
	GLuint _VBOsize;
	GLuint _EBOsize;
};


#endif // !MESHRENDERER_H
