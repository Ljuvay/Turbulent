#ifndef TERRAIN_H
#define TERRAIN_H

#include <glm/glm.hpp>

#include <vector>
#include <memory>

#include "chunk/chunk.h"

constexpr int DEFAULT_TERRAIN_SIZE = 4000;
constexpr int DEFAULT_TERRAIN_HEIGHT = 200;
constexpr int DEFAULT_TERRAIN_SUBDIVISIONS = 20;
constexpr int DEFAULT_TERRAIN_OCTAVES = 8;

struct terrSettings
{
	int terrSize = DEFAULT_TERRAIN_SIZE;
	int terrHeight = DEFAULT_TERRAIN_HEIGHT;
	int terrSubDiv = DEFAULT_TERRAIN_SUBDIVISIONS;
	int terrOctaves = DEFAULT_TERRAIN_OCTAVES;
};

class terrain
{
public:
	void buildTerrain();
	void rebuildTerrain();
	void deleteTerrain();

	void generateSeed();

private:
	terrSettings t_Settings;
	std::vector<Chunk> Chunks;
	int seed;
};

#endif //!TERRAIN_H