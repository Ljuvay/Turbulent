#ifndef BILLBOARD_H
#define BILLBOARD_H

#include <glm/glm.hpp>
#include <glad/glad.h>

#include <vector>

#include "vertex.h"

struct billBoard
{
	float quadVerts[8] = {
		-1.0f, -1.0f,
		 1.0f, -1.0f,
		 1.0f,  1.0f,
		-1.0f,  1.0f
	};
	unsigned int quadInds[6] =
	{
		0, 1, 2,
		2, 3, 0
	};
};

#endif // !BILLBOARD_H