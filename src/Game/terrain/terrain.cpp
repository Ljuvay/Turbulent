#include <glm/glm.hpp>

#include <vector>

#include "terrain.h"
#include "chunk.h"

constexpr int CHUNK_DEFAULT_RESOLUTION = 10;

terrain::terrain(const terrSettings& settings)
{
	t_Settings = settings;
}

void terrain::buildTerrain()
{
	int XChunksAmount = t_Settings.terrSubDiv;
	int YChunksAmount = t_Settings.terrSubDiv;

	int XChunksSize = t_Settings.terrSize / t_Settings.terrSubDiv;
	int YChunksSize = XChunksSize;


	//Build Chunks from -ChunkSize.xz to ChunkSize.xz
	for (int XChunkIter = -XChunksAmount / 2; XChunkIter < XChunksAmount / 2; XChunkIter++)
	{
		for (int YChunkIter = -YChunksAmount / 2; YChunkIter < YChunksAmount / 2; YChunkIter++)
		{
			float chunkSize = t_Settings.terrSize / t_Settings.terrSubDiv;

			//Get pos for chunk based off iterators
			glm::vec2 chunkStartPos; // We start at bottom left (-, -) and go to (+, +)
			chunkStartPos.x = (XChunkIter * chunkSize);
			chunkStartPos.y = (YChunkIter * chunkSize);

			//Chunk terrChunk(chunkStartPos, chunkSize, CHUNK_DEFAULT_RESOLUTION);
			Chunks.push_back(std::make_unique<Chunk>(chunkStartPos, XChunksSize, CHUNK_DEFAULT_RESOLUTION, t_Settings));
		}
	}
}