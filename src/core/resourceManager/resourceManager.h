#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "glm/glm.hpp"

#include <vector>
#include <unordered_map>
#include <memory>

#include "mesh.h"
#include "shader.h"
#include "vertex.h"
#include "shaderSystem.h"
#include "meshSystem.h"
#include "materialSystem.h"
#include "textureSystem.h"

class ResourceManager {
public:
	ResourceManager();

	void loadResources();

	meshSystem& meshes() { return *rm_meshSystem; }
	shaderSystem& shaders() { return *rm_shaderSystem; }
	textureSystem& textures() { return *rm_textureSystem; }

	void createEmptyParticleGroup(const std::string& collectionName);
	void addParticles(const std::vector<Vertex>& particles, const std::string& collectionName);
	void deleteParticles(const std::string& collectionName);
	void updateParticles(const std::vector<Vertex>& particles, const std::string& collectionName);
	uint32_t particleIDfromName(const std::string& collectionName);

	bool hasParticles(const std::string& containerName);

	std::vector<Vertex>& getParticleData(const std::string& collectionName);

private:
	std::unique_ptr<shaderSystem> rm_shaderSystem;
	std::unique_ptr<meshSystem> rm_meshSystem;
	std::unique_ptr<textureSystem> rm_textureSystem;

	void loadMeshGroup();
	void loadShaderGroup();
	void loadParticleGroup();
	void loadTextureGroup();

	std::unordered_map<uint32_t, std::vector<Vertex>> particleContainer;
	std::unordered_map<std::string, uint32_t> particleIdContainer;
	std::vector<Vertex> particles;
};


#endif // !RESOURCEMANAGER_H