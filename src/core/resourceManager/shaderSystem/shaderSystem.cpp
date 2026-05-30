#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>

#include "shaderSystem.h"
#include "shader.h"
#include "resourceTargets.h"

uint32_t shaderSystem::addShader(const std::string& shaderPath_vs, const std::string& shaderPath_fs, const std::string& shaderName)
{
	std::cout << "Trying to add Shader: \""
		<< shaderName
		<< "\" to scene | STATUS: ";

	const auto it = shaderIdContainer.find(shaderName);

	if (it != shaderIdContainer.end())
	{
		std::cerr << "FAILURE (ALREADY EXIST)"
			<< std::endl;

		return it->second;
	}

	uint32_t id = nextShaderID++;

	shaderIdContainer.emplace(shaderName, id);

	shaderContainer.emplace(id,
		std::make_unique<Shader>(
			shaderPath_vs.c_str(),
			shaderPath_fs.c_str()
		)
	);

	std::cout << "SUCCESS" << std::endl;

	return id;
}

void shaderSystem::updateShader(const std::string& shaderPath_vs, const std::string& shaderPath_fs, const std::string& shaderName)
{
	std::cout << "Updating Shader: \""
		<< shaderName
		<< "\" STATUS: ";

	if (!hasShader(shaderName))
	{
		std::cout << "FAILURE (DOESNT EXIST)"
			<< std::endl;
		return;
	}

	const uint32_t id = shaderIDfromName(shaderName);

	shaderContainer[id] = std::make_unique<Shader>(
		shaderPath_vs.c_str(),
		shaderPath_fs.c_str()
	);

	std::cout << "SUCCESS" << std::endl;
}

uint32_t shaderSystem::shaderIDfromName(const std::string& shaderName)
{
	return shaderIdContainer.at(shaderName);
}

bool shaderSystem::hasShader(const std::string& shaderName)
{
	return shaderIdContainer.find(shaderName) != shaderIdContainer.end();
}

Shader& shaderSystem::getShaderData(const uint32_t& shaderID)
{
	auto it = shaderContainer.find(shaderID);

	if (it == shaderContainer.end())
	{
		std::cerr << "ERROR: shader ID does not exist"
			<< std::endl;
		throw std::runtime_error("Invalid shader ID");
	}

	return *it->second;
}

void shaderSystem::loadShaderGroup()
{
	for (const auto& t : SHADER_TARGETS)
	{
		addShader(std::string(t.vs), std::string(t.fs), std::string(t.name));
	}
}
