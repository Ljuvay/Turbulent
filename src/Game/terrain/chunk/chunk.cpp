#include <glm/glm.hpp>

#include <vector>

#include "chunk.h"
#include "perlin.h"

Chunk::Chunk(glm::ivec2 coord, int size, int resolution, const terrSettings& settings) :
	coord(coord),
	size(size),
	resolution(resolution),
	t_settings(settings)
{
	float chunkSteps = size / resolution;
	float stepSize = (float)size / (float)resolution;

	noise::Perlin perlin;
	float scale = 0.005f;

	for (float i = 0; i < resolution + 1; i++)
	{
		for (float j = 0; j < resolution + 1; j++)
		{
			float worldX = coord.x + i * stepSize;
			float worldZ = coord.y + j * stepSize;

			float baseNoise = perlin.OctavePerlin(worldX * scale, 0, worldZ * scale, 4, 0.5f);
			float mountainMask = perlin.OctavePerlin(worldX * scale * 0.3f, 0.5f, worldZ * scale * 0.3f, 2, 0.5f);
			mountainMask = pow(mountainMask, 5.0f);
			float mountainNoise = perlin.OctavePerlin(worldX * scale * 2.0f, 1.0f, worldZ * scale * 2.0f, 8, 0.6f);
			float height = (baseNoise * 40.0f) + (mountainMask * mountainNoise * t_settings.terrHeight);
			
			/*float perlinVal = (perlin.OctavePerlin(worldX * scale, 0, worldZ * scale, t_settings.terrOctaves, 0.5f) - 0.5f) * 2.0f;
			perlinVal = pow(perlinVal, 4.0f);
			float height = (perlinVal - 0.01f) * t_settings.terrHeight;
			*/

			terrVert tVert{
				{worldX, height, worldZ}, // Position
				{0.0f, 0.0f, 0.0f},	// Normal
				{0.0f, 0.0f}		// UV
			};
			terrainVertices.push_back(tVert);
		}
	}

	for (int i = 0; i < resolution; ++i) 
	{
		for (int j = 0; j < resolution; ++j)
		{
			int topLeft = i * (resolution + 1) + j;
			int topRight = topLeft + 1;
			int bottomLeft = (i + 1) * (resolution + 1) + j;
			int bottomRight = bottomLeft + 1;

			indices.push_back(topLeft);
			indices.push_back(topRight);
			indices.push_back(bottomLeft);

			indices.push_back(topRight);
			indices.push_back(bottomRight);
			indices.push_back(bottomLeft);
		}
	}
}
