#include <glm/glm.hpp>

#include <vector>

#include "chunk.h"
#include "perlin.h"
#include "vertex.h"

Chunk::Chunk(glm::ivec2 coord, int size, int32_t seed, int resolution, const terrSettings& settings) :
	coord(coord),
	size(size),
	resolution(resolution),
	t_settings(settings)
{
	float chunkSteps = size / resolution;
	float stepSize = (float)size / (float)resolution;

	noise::Perlin perlin;
	float scale = 0.002f;

	for (float i = 0; i < resolution + 1; i++)
	{
		for (float j = 0; j < resolution + 1; j++)
		{
			float worldX = coord.x + i * stepSize;
			float worldZ = coord.y + j * stepSize;

			float baseNoise = perlin.OctavePerlin(worldX * scale, 0, worldZ * scale, 4, 0.5f);
			float mountainMask = perlin.OctavePerlin(worldX * scale * 0.3f, 0.5f, worldZ * scale * 0.3f, 2, 0.5f);
			mountainMask = pow(mountainMask, 5.5f);
			float mountainNoise = perlin.OctavePerlin(worldX * scale * 2.0f, 1.0f, worldZ * scale * 2.0f, 8, 0.6f);
			float height = (baseNoise * 30.0f) + (mountainMask * mountainNoise * t_settings.terrHeight);
			
			/*float perlinVal = (perlin.OctavePerlin(worldX * scale, 0, worldZ * scale, t_settings.terrOctaves, 0.5f) - 0.5f) * 2.0f;
			perlinVal = pow(perlinVal, 4.0f);
			float height = (perlinVal - 0.01f) * t_settings.terrHeight;
			*/

			Vertex tVert{
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

void Chunk::build()
{
	/*
	auto& chunks = df_terrain->getChunks();
	auto& chunkVerts = chunks[0]->getVertices();
	auto& chunkIndices = chunks[0]->getIndices();

	std::vector<Vertex> tempVerts;
	std::vector<GLuint> tempIndices;
	GLuint vertOffset = 0;
	
	for (auto& chunk : df_terrain->getChunks())
	{
		for (auto& tv : chunk->getVertices())
		{
			Vertex v;
			v.position = tv.pos;
			v.normal = tv.norm;
			v.uv = tv.uv;
			v.color = { 0.0f, 1.0f, 1.0f };
			tempVerts.push_back(v);
		}
		for (auto& idx : chunk->getIndices())
		{
			tempIndices.push_back(idx + vertOffset);
		}
		vertOffset += chunk->getVertices().size();
	}
	*/
}