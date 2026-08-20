#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <glad/glad.h>

#include <iostream>

class frameBuffer
{
public:
	void create(int width, int height, bool depthOnly);
	void bind();
	void unbind();
	void cleanup();

	GLuint getDepthTexture() const { return depthTex; }
	GLuint getColorTexture() const { return colorTex; }

	int getWidth() const { return this->width; }
	int getHeight() const { return this->height; }

private:
	bool depthOnly = false;

	GLuint fbo = 0;
	GLuint rbo = 0;
	GLuint depthTex = 0;
	GLuint colorTex = 0;
	int width, height;
};

#endif // !FRAMEBUFFER_H
