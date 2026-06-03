#include <stb_image.h>

#include "textureSystem.h"

void textureSystem::loadTextures()
{
	for (const auto& t : TEXTURE_TARGETS)
	{
		addTexture(std::string(t.path), std::string(t.name));
		uploadTexture(std::string(t.name));
	}
}

uint32_t textureSystem::addTexture(const std::string& texturePath, const std::string& textureName)
{
	std::cout << "Trying to add Texture: \""
		<< textureName
		<< "\" to scene | STATUS: ";

	const auto it = textureIdContainer.find(textureName);

	if (it != textureIdContainer.end())
	{
		std::cerr << "FAILURE (ALREADY EXIST)"
			<< std::endl;

		return it->second;
	}

	uint32_t id = nextTextureID++;

	textureIdContainer.emplace(textureName, id);

	auto tex = std::make_unique<Texture>();
	tex->name = textureName;
	tex->path = texturePath;
	textureContainer.emplace(id, std::move(tex));

	std::cout << "SUCCESS" << std::endl;

	return id;
}

void textureSystem::deleteTexture(const std::string& textureName)
{
	std::cout << "DELETE TEXTURE NOT DONE" << std::endl;
}

uint32_t textureSystem::textureIDfromName(const std::string& textureName)
{
	return textureIdContainer.at(textureName);
}

bool textureSystem::hasTexture(const std::string& textureName)
{
	return textureIdContainer.find(textureName) != textureIdContainer.end();
}

bool textureSystem::hasGPUTexture(const uint32_t id)
{
	return gpuTextureContainer.find(id) != gpuTextureContainer.end();
}

uint32_t textureSystem::uploadTexture(const std::string& textureName)
{
	uint32_t id = textureIdContainer.at(textureName);
	Texture& texture = *textureContainer.at(id);

	if (gpuTextureContainer.find(id) != gpuTextureContainer.end())
	{
		return id;
	}

	gpuTexture gpu;

	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(texture.path.c_str(), &gpu.width, &gpu.height, &gpu.channels, 0);
	if (!data)
	{
		std::cerr << "Failed to load texture at: " << texture.path << std::endl;
		return 0;
	}

	GLenum format = (gpu.channels == 4) ? GL_RGBA : GL_RGB;

	glGenTextures(1, &gpu.glID);
	glBindTexture(GL_TEXTURE_2D, gpu.glID);
	glTexImage2D(GL_TEXTURE_2D, 0, format, gpu.width, gpu.height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_image_free(data);
	gpuTextureContainer.emplace(id, gpu);

	if (gpuTextureContainer.find(id) == gpuTextureContainer.end())
	{
		std::cerr << "GPU texture not created for ID: " << id << std::endl;
	}

	return id;
}

GLuint textureSystem::getGLTextureID(uint32_t id)
{
	return gpuTextureContainer.at(id).glID;
}