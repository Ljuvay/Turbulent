#include <glm/glm.hpp>

#include <vector>
#include <random>
#include <limits.h>

#include "terrain.h"
#include "chunk.h"

constexpr int CHUNK_DEFAULT_RESOLUTION = 30;

terrain::terrain(const terrSettings& settings)
{
	t_Settings = settings;
	generateSeed();
}

void terrain::buildTerrain()
{
	int XChunksAmount = t_Settings.terrSubDiv;
	int YChunksAmount = t_Settings.terrSubDiv;

	int XChunksSize = t_Settings.terrSize / t_Settings.terrSubDiv;
	int YChunksSize = XChunksSize;


	//Build Chunks from -ChunkSize.xz to ChunkSize.xz
	for (int XChunkIter = -XChunksAmount / 2; XChunkIter < XChunksAmount / 2; XChunkIter++) {
		for (int YChunkIter = -YChunksAmount / 2; YChunkIter < YChunksAmount / 2; YChunkIter++) {
			float chunkSize = t_Settings.terrSize / t_Settings.terrSubDiv;

			//Get pos for chunk based off iterators
			glm::vec2 chunkStartPos; // We start at bottom left (-, -) and go to (+, +)
			chunkStartPos.x = (XChunkIter * chunkSize);
			chunkStartPos.y = (YChunkIter * chunkSize);

			Chunks.push_back(std::make_unique<Chunk>(chunkStartPos, XChunksSize, seed, CHUNK_DEFAULT_RESOLUTION, t_Settings));
		}
	}
}

void terrain::generateSeed() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int64_t> seedRange(std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::max());
	seed = seedRange(gen);
}

std::pair<std::vector<Vertex>, std::vector<GLuint>> terrain::toMeshData() const
{
	std::vector<Vertex> verts;
	std::vector<GLuint> indices;
	GLuint vertOffset = 0;

	for (auto& chunk : Chunks)
	{
		for (auto& tv : chunk->getVertices())
		{
			verts.push_back(tv);
		}
		for (auto& idx : chunk->getIndices())
		{
			indices.push_back(idx + vertOffset);
		}
		vertOffset += chunk->getVertices().size();
	}

	return { verts, indices };
}