#ifndef TERRAINMANAGER_H
#define TERRAINMANAGER_H

#include <glm/glm.hpp>

#include <vector>
#include <iostream>
#include <memory>

#include "terrSettings.h"
#include "terrain.h"

struct noisePass
{

};

class terrainManager
{
public:


private:
	std::unique_ptr<terrain> m_terrain;
	std::vector<noisePass> m_noise_passes;
	terrSettings m_settings;

	uint32_t seed;
};

#endif // !TERRAINMANAGER_H