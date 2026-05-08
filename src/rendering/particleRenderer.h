#ifndef PARTICLERENDERER_H
#define PARTICLERENDERER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>
#include <iostream>
#include <cstddef>

#include "vertex.h"

class particleRenderer
{
public:
	particleRenderer()
	{
		float quad[] = {
			-1.0f, -1.0f,
			 1.0f, -1.0f,
			 1.0f,  1.0f,
			-1.0f,  1.0f
		};

		unsigned int quadInds[] = {
			0, 1, 2,
			2, 3, 0
		};

		glGenVertexArrays(1, &_VAO);
		glBindVertexArray(_VAO);

		glGenBuffers(1, &_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, _VBO);
		glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

		glGenBuffers(1, &_quadVBO);
		glBindBuffer(GL_ARRAY_BUFFER, _quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

		glGenBuffers(1, &_quadEBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _quadEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadInds), quadInds, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, _VBO);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		glEnableVertexAttribArray(1);
		glVertexAttribDivisor(1, 1);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
		glEnableVertexAttribArray(2);
		glVertexAttribDivisor(2, 1);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
		glEnableVertexAttribArray(3);
		glVertexAttribDivisor(3, 1);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		glEnableVertexAttribArray(4);
		glVertexAttribDivisor(4, 1);
		glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, radius));
		glEnableVertexAttribArray(5);
		glVertexAttribDivisor(5, 1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		_VBOsize = 0;
		_vertexCount = 0;
	}

	~particleRenderer()
	{
		glDeleteVertexArrays(1, &_VAO);
		glDeleteBuffers(1, &_VBO);
		glDeleteBuffers(1, &_quadEBO);
		glDeleteBuffers(1, &_quadVBO);
	}

	void updateGPU(const std::vector<Vertex>& vertices)
	{
		GLuint newSizeV = sizeof(Vertex) * vertices.size();

		glBindVertexArray(_VAO);

		glBindBuffer(GL_ARRAY_BUFFER, _VBO);
		if (newSizeV != _VBOsize) {
			glBufferData(GL_ARRAY_BUFFER, newSizeV, vertices.data(), GL_DYNAMIC_DRAW);
			_VBOsize = newSizeV;
		}
		else if (newSizeV > 0) {
			glBufferSubData(GL_ARRAY_BUFFER, 0, newSizeV, vertices.data());
		}

		glBindVertexArray(0);
		_vertexCount = static_cast<GLsizei>(vertices.size());
	}

	void drawPoints() const
	{
		glBindVertexArray(_VAO);
		glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, _vertexCount);
		glBindVertexArray(0);
	}

private:
	GLuint _VAO;
	GLuint _VBO;
	GLuint _quadVBO;
	GLuint _quadEBO;
	GLuint _VBOsize;
	GLsizei _vertexCount;
};


#endif // !PARTICLERENDERER_H