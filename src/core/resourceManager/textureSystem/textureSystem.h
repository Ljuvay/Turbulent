#ifndef TEXTURESYSTEM_H
#define TEXTURESYSTEM_H

#include <glm/glm.hpp>
#include <glad/glad.h>

#include <vector>
#include <unordered_map>
#include <memory>
#include <string>
#include <iostream>

#include "texture.h"
#include "resourceTargets.h"

class textureSystem
{
public:
	void loadTextures();

	uint32_t addTexture(const std::string& texturePath, const std::string& textureName);
	void deleteTexture(const std::string& textureName);
	
	uint32_t textureIDfromName(const std::string& textureName);

	bool hasTexture(const std::string& textureName);
	bool hasGPUTexture(const uint32_t id);

	uint32_t uploadTexture(const std::string& textureName);
	GLuint getGLTextureID(uint32_t id);
private:
	uint32_t nextTextureID = 1;

	std::unordered_map<uint32_t, std::unique_ptr<Texture>> textureContainer;
	std::unordered_map<uint32_t, gpuTexture> gpuTextureContainer;
	std::unordered_map<std::string, uint32_t> textureIdContainer;
};

#endif // !TEXTURESYSTEM_H