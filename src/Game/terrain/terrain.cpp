#include <glm/glm.hpp>

#include <vector>

#include "terrain.h"
#include "chunk.h"

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
			Chunk terrChunk();
		}
	}
}