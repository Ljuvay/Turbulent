#ifndef CHUNK_H
#define CHUNK_H

#include <glm/glm.hpp>

#include <vector>

class Chunk
{
public:
	Chunk(glm::vec3 chunkPos);

	Chunk(const Chunk&) = delete;
	Chunk& operator = (const Chunk&) = delete;

	Chunk(Chunk&&) = delete;
	Chunk& operator=(Chunk&&) = delete;

	void buildChunk(glm::vec3 chunkPos);

private:
	std::vector<glm::vec3> chunkPoints;
	glm::vec2 chunkPos;
};

#endif // !CHUNK_H
