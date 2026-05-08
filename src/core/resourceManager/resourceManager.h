#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "glm/glm.hpp"

#include <vector>
#include <unordered_map>
#include <memory>

#include "mesh.h"
#include "shader.h"
#include "vertex.h"

class ResourceManager {
public:
	void addMesh(const std::string& meshPath, const std::string& meshName);
	void deleteMesh(const std::string& meshName);
	void updateMesh(const std::string& meshName, Mesh newMesh);

	void addParticles(const std::string& collectionName);
	void deleteParticles(const std::string& collectionName);
	void updateParticles(const std::vector<Vertex>& particles, const std::string& collectionName);

	void addShader(const std::string& shaderPath_vs, const std::string& shaderPath_fs, const std::string& shaderName);
	void deleteShader(const std::string& shaderName);
	void updateShader(const std::string& shaderPath_vs, const std::string& shaderPath_fs, const std::string& shaderName);

	bool hasMesh(const std::string& meshName);
	bool hasShader(const std::string& shaderName);
	bool hasParticles(const std::string& containerName);

	Shader& getShaderData(const std::string& shaderName);
	Mesh& getMeshData(const std::string& meshName);
	std::vector<Vertex>& getParticleData(const std::string& collectionName);

private:
	std::unordered_map<std::string, Mesh> meshContainer;
	std::unordered_map<std::string, std::unique_ptr<Shader>> shaderContainer;
	std::unordered_map<std::string, std::vector<Vertex>> particleCollectionContainer;
	std::vector<Vertex> particles;
};


#endif // !RESOURCEMANAGER_H

//std::unordered_map<std::string, std::unique_ptr<Shader>> _Shaders;
//_Shaders.emplace("boundingBoxShader", std::make_unique<Shader>("resources/shaders/boundingBox_vs.glsl", "resources/shaders/boundingBox_fs.glsl"));
//_Shaders.emplace("SPHParticleShader", std::make_unique<Shader>("resources/shaders/SPHParticle_vs.glsl", "resources/shaders/SPHParticle_fs.glsl"));
//_Shaders.emplace("defaultMeshShader", std::make_unique<Shader>("resources/shaders/defaultMesh_vs.glsl", "resources/shaders/defaultMesh_fs.glsl"));
