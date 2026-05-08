#ifndef BOUNDINGBOXRENDERER_H
#define BOUNDINGBOXRENDERER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>

class BoundingBoxRenderer
{
public:
	BoundingBoxRenderer();
	~BoundingBoxRenderer();

	void updateGPU(const std::vector<glm::vec3>& vertices, const std::vector<GLuint>& indices);
	void drawCube() const;

private:
	GLuint _VAO;
	GLuint _VBO;
	GLuint _EBO;
	GLuint _indexCount;
	GLuint _VBOsize;
	GLuint _EBOsize;
};

#endif // !BOUNDINGBOXRENDERER_H
