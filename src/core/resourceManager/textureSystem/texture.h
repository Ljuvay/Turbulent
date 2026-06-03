#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>

#include <string>

struct Texture
{
	std::string name;
	std::string path;
};

struct gpuTexture
{
	GLuint glID = 0;
	int width = 0;
	int height = 0;
	int channels = 0;
};

#endif // !TEXTURE_H