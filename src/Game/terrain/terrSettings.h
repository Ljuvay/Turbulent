#ifndef TERRSETTINGS_H
#define TERRSETTINGS_H

constexpr int DEFAULT_TERRAIN_SIZE = 1000;
constexpr int DEFAULT_TERRAIN_HEIGHT = 2000;
constexpr int DEFAULT_TERRAIN_SUBDIVISIONS = 40;
constexpr int DEFAULT_TERRAIN_OCTAVES = 8;

struct terrSettings
{
	int terrSize = DEFAULT_TERRAIN_SIZE;
	int terrHeight = DEFAULT_TERRAIN_HEIGHT;
	int terrSubDiv = DEFAULT_TERRAIN_SUBDIVISIONS;
	int terrOctaves = DEFAULT_TERRAIN_OCTAVES;
};

#endif // !TERRSETTINGS_H
