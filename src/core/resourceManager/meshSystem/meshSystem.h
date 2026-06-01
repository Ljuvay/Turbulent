#ifndef MESHSYSTEM_H
#define MESHSYSTEM_H

#include <glm/glm.hpp>

#include <vector>
#include <unordered_map>
#include <memory>
#include <string>
#include <iostream>

#include "gpuMesh.h"
#include "mesh.h"
#include "vertex.h"
#include "fileLoader.h"

class meshSystem
{
public:
	void loadMeshGroup();

	uint32_t addMesh(const std::string& meshPath, const std::string& meshName);
	uint32_t addMesh(const Mesh& mesh, const std::string& meshName);
	void deleteMesh(const std::string& meshName);
	void updateMesh(const std::string& meshName, Mesh newMesh);
	
	uint32_t meshIDfromName(const std::string& meshName);

	bool hasMesh(const std::string& meshName);
	bool hasGPUMesh(uint32_t id);

	Mesh& getMeshData(const std::string& meshName);
	std::vector<std::string> getMeshNames() const;

	gpuMesh& getGPUMesh(uint32_t id); // we need this for flush
	uint32_t uploadMesh(const std::string& meshName); // Send it to da GPU
private:
	uint32_t nextMeshID = 1;

	std::unordered_map<uint32_t, std::unique_ptr<Mesh>> meshContainer;
	std::unordered_map<uint32_t, gpuMesh> gpuMeshContainer;
	std::unordered_map<std::string, uint32_t> meshIdContainer;
};

#endif // !MESHSYSTEM_H