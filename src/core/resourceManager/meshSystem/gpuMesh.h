#ifndef GPUMESH_H
#define GPUMESH_H

#include <glad/glad.h>
#include <glm/glm.hpp>

struct gpuMesh
{
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	GLuint indexCount;

	GLenum usage = GL_STATIC_DRAW;

	void destroy()
	{
		if (VBO) { glDeleteBuffers(1, &VBO); }
		if (EBO) { glDeleteBuffers(1, &EBO); }
		if (VAO) { glDeleteVertexArrays(1, &VAO); }

		VBO = 0;
		EBO = 0;
		EBO = 0;
		indexCount = 0;
	}
};

#endif // !GPUMESH_H