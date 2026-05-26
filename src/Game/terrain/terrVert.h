#ifndef TERRVERT_H
#define TERRVERT_H

#include <glm/glm.hpp>

struct terrVert
{
	glm::vec3 pos = { 0.0f, 0.0f, 0.0f };
	glm::vec3 norm = { 0.0f, 0.0f, 0.0f };
	glm::vec2 uv = { 0.0f, 0.0f };
};

#endif // !TERRVERT_H
