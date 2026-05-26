#ifndef CHUNK_H
#define CHUNK_H

#include <glm/glm.hpp>

#include <vector>

#include "terrVert.h"
#include "terrSettings.h"

class Chunk
{
public:
	Chunk(glm::ivec2 coord, int size, int resolution, const terrSettings& settings);

	Chunk(const Chunk&) = delete;
	Chunk& operator = (const Chunk&) = delete;

	Chunk(Chunk&&) = delete;
	Chunk& operator=(Chunk&&) = delete;

	void build();

	const std::vector<terrVert>& getVertices() const { return terrainVertices; }
	const std::vector<uint32_t>& getIndices() const { return indices; }

private:
	glm::ivec2 coord;
	int size;
	int resolution;
	terrSettings t_settings;

	std::vector<float> heightmap;
	std::vector<terrVert> terrainVertices;
	std::vector<uint32_t> indices;
};

#endif // !CHUNK_H
