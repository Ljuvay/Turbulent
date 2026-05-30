#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>

#include "gpuMesh.h"
#include "mesh.h"
#include "vertex.h"
#include "meshSystem.h"
#include "resourceTargets.h"

uint32_t meshSystem::addMesh(const std::string& meshPath, const std::string& meshName)
{
	std::cout << "Trying to add Mesh: \""
		<< meshName
		<< "\" to scene | STATUS: ";

	const auto it = meshIdContainer.find(meshName);

	if (it != meshIdContainer.end())
	{
		std::cerr << "FAILURE (ALREADY EXIST)"
			<< std::endl;

		return it->second;
	}

	uint32_t id = nextMeshID++;

	meshIdContainer.emplace(meshName, id);

	meshContainer.emplace(id,
		std::make_unique<Mesh>(fileLoader::loadObj(meshPath))
	);

	std::cout << "SUCCESS" << std::endl;

	return id;
}

uint32_t meshSystem::addMesh(const Mesh& mesh, const std::string& meshName)
{
	auto it = meshIdContainer.find(meshName);
	if (it != meshIdContainer.end())
	{
		return it->second;
	}

	uint32_t id = nextMeshID;
	meshIdContainer.emplace(meshName, id);
	meshContainer.emplace(id, std::make_unique<Mesh>(mesh));
	return id;
}

void meshSystem::deleteMesh(const std::string& meshName)
{
	std::cout << "DELETE MESH NOT DONE" << std::endl;
}

void meshSystem::updateMesh(const std::string& meshName, Mesh newMesh)
{
	std::cout << "Updating Mesh: \""
		<< meshName
		<< "\" STATUS: ";

	if (!hasMesh(meshName))
	{
		std::cout << "FAILURE (DOESNT EXIST)"
			<< std::endl;
		return;
	}

	const uint32_t id = meshIDfromName(meshName);

	meshContainer[id] = std::make_unique<Mesh>(
		newMesh
	);

	std::cout << "SUCCESS" << std::endl;
}

uint32_t meshSystem::meshIDfromName(const std::string& meshName)
{
	return meshIdContainer.at(meshName);
}

bool meshSystem::hasMesh(const std::string& meshName)
{
	return meshIdContainer.find(meshName) != meshIdContainer.end();

}

Mesh& meshSystem::getMeshData(const std::string& meshName)
{
	auto it = meshIdContainer.find(meshName);

	if (it == meshIdContainer.end())
	{
		std::cerr << "ERROR: mesh with name \""
			<< meshName
			<< "\"does not exist"
			<< std::endl;
		throw std::runtime_error("Invalid mesh ID");
	}

	uint32_t id = it->second;

	auto it2 = meshContainer.find(id);

	if (it2 == meshContainer.end())
	{
		std::cerr << "ERROR: mesh ID does not exist"
			<< std::endl;
		throw std::runtime_error("Invalid mesh ID");
	}

	return *it2->second;
}

gpuMesh& meshSystem::getGPUMesh(uint32_t id)
{
	auto it = gpuMeshContainer.find(id);

	if (it == gpuMeshContainer.end())
	{
		throw std::runtime_error("GPU mesh not uploaded");
	}

	return it->second;
}

bool meshSystem::hasGPUMesh(uint32_t id)
{
	return gpuMeshContainer.find(id) != gpuMeshContainer.end();
}

uint32_t meshSystem::uploadMesh(const std::string& meshName)
{
	uint32_t id = meshIdContainer.at(meshName);
	Mesh& mesh = *meshContainer.at(id);

	if (gpuMeshContainer.find(id) != gpuMeshContainer.end())
	{
		return id;
	}

	gpuMesh gpu;

	// Buffers and shit
	glGenVertexArrays(1, &gpu.VAO);
	glBindVertexArray(gpu.VAO);

	glGenBuffers(1, &gpu.VBO);
	glBindBuffer(GL_ARRAY_BUFFER, gpu.VBO);
	glBufferData(GL_ARRAY_BUFFER,
		mesh.vertices.size() * sizeof(Vertex),
		mesh.vertices.data(),
		gpu.usage);

	glGenBuffers(1, &gpu.EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gpu.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		mesh.indices.size() * sizeof(uint32_t),
		mesh.indices.data(),
		gpu.usage);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	glEnableVertexAttribArray(2);

	gpu.indexCount = static_cast<GLsizei>(mesh.indices.size());

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	gpuMeshContainer[id] = gpu;

	if (gpuMeshContainer.find(id) == gpuMeshContainer.end())
	{
		std::cerr << "GPU mesh not created for ID: " << id << std::endl;
	}

	return id;
}

void meshSystem::loadMeshGroup()
{
	for (const auto& t : MESH_TARGETS)
	{
		addMesh(std::string(t.path), std::string(t.name));
		uploadMesh(std::string(t.name));
	}
}