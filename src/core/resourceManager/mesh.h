#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>

#include <vector>
#include <string>

#include "vertex.h"

struct Mesh
{
	std::string name;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Vertex> originalVertices;
};


#endif // !MESH_H