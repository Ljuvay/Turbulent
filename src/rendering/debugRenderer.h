#ifndef DEBUGRENDERER_H
#define DEBUGRENDERER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>

#include "resourceManager.h"

class debugRenderer
{
public:
	void init(ResourceManager* rm);
	void drawGrid(const glm::mat4& view, const glm::mat4& proj, const glm::vec3& camPos);

private:
	GLuint m_gridVAO;
	ResourceManager* m_rm;
};

#endif // !DEBUGRENDERER_H
