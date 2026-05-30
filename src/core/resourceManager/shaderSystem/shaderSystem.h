#ifndef SHADERSYSTEM_H
#define SHADERSYSTEM_H

#include <glm/glm.hpp>

#include <vector>
#include <unordered_map>
#include <memory>
#include <string>
#include <iostream>

#include "shader.h"

class shaderSystem
{
public:
	void loadShaderGroup();

	uint32_t addShader(const std::string& shaderPath_vs, const std::string& shaderPath_fs, const std::string& shaderName);
	void updateShader(const std::string& shaderPath_vs, const std::string& shaderPath_fs, const std::string& shaderName);
	uint32_t shaderIDfromName(const std::string& shaderName);

	bool hasShader(const std::string& shaderName);

	Shader& getShaderData(const uint32_t& shaderID);
private:
	uint32_t nextShaderID = 1;

	std::unordered_map<uint32_t, std::unique_ptr<Shader>> shaderContainer;
	std::unordered_map<std::string, uint32_t> shaderIdContainer;
};

#endif // !SHADERSYSTEM_H