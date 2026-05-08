#ifndef SOFTBODY_H
#define SOFTBODY_H

#include <glm/glm.hpp>

#include <vector>

#include "vertex.h"
#include "rigidBody.h"

struct spring
{
	rigidBodyNode* vertAnc;
	rigidBodyNode* vertRB;
	rigidBody* rbAnc;
	rigidBody* rbPlat;
	float restLength;
	float currLength;
};

#endif // !SOFTBODY_H