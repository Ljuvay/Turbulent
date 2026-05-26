#ifndef TERRAIN_H
#define TERRAIN_H

#include <glm/glm.hpp>

#include <vector>
#include <memory>

#include "chunk.h"
#include "terrVert.h"
#include "terrSettings.h"

class terrain
{
public:
	terrain(const terrSettings& settings);

	void buildTerrain();
	void rebuildTerrain(const terrSettings& newSettings);
	void deleteTerrain();

	void generateSeed();

	float sampleHeight(float x, float z) const;
	Chunk* getChunkAt(int worldX, int worldZ);
	const std::vector<std::unique_ptr<Chunk>>& getChunks() const { return Chunks; }

private:
	void createChunks();

	terrSettings t_Settings;
	std::vector<std::unique_ptr<Chunk>> Chunks;
	uint32_t seed;
};

#endif //!TERRAIN_H