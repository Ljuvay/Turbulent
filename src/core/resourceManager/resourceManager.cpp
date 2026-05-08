#include "resourceManager.h"
#include "fileLoader.h"
#include "mesh.h"

#include <string>
#include <iostream>
#include <unordered_map>

// Mesh

void ResourceManager::addMesh(const std::string& meshPath, const std::string& meshName) {
	std::cout << "Trying to add Mesh: \"" << meshName << "\" to scene | STATUS: ";
	if (meshContainer.find(meshName) != meshContainer.end()) {
		std::cerr << "FAILURE \"" << meshName << "\" ALREADY EXIST" << std::endl;
		return;
	}
	std::cout << "SUCCESS" << std::endl;
	meshContainer.emplace(meshName, fileLoader::loadObj(meshPath));
}

void ResourceManager::deleteMesh(const std::string& meshName) {
	if (meshContainer.find(meshName) == meshContainer.end()) {
		std::cerr << "ERROR::ERROR MESH " << meshName << " DOES NOT EXIST IN THIS SCENE" << std::endl;
	}
	meshContainer.erase(meshName);
}

void ResourceManager::updateMesh(const std::string& meshName, Mesh newMesh) {
	meshContainer[meshName] = newMesh;
}

// Particle

void ResourceManager::addParticles(const std::string& collectionName) {
	std::cout << "Trying to add Particle Collection: \"" << collectionName << "\" to scene | STATUS: ";
	if (particleCollectionContainer.find(collectionName) != particleCollectionContainer.end()) {
		std::cerr << "FAILURE \"" << collectionName << "\" ALREADY EXIST" << std::endl;
	}
	std::cout << "SUCCESS" << std::endl;

	std::vector<Vertex> particles;
	particleCollectionContainer.emplace(collectionName, particles);
}

void ResourceManager::deleteParticles(const std::string& collectionName) {
	if (particleCollectionContainer.find(collectionName) == particleCollectionContainer.end()) {
		std::cerr << "ERROR:ERROR PARTICLE COLLECTION " << collectionName << " DOES NOT EXIST IN THIS SCENE" << std::endl;
	}
}

void ResourceManager::updateParticles(const std::vector<Vertex>& particles, const std::string& collectionName) {
	particleCollectionContainer[collectionName] = particles;
}


// Shader

void ResourceManager::addShader(const std::string& shaderPath_vs, const std::string& shaderPath_fs, const std::string& shaderName) {
	std::cout << "Trying to add Shader: \"" << shaderName << "\" to scene | STATUS: ";
	if (shaderContainer.find(shaderName) != shaderContainer.end()) {
		std::cerr << "FAILURE \"" << shaderName << "\" ALREADY EXIST" << std::endl;
	}
	shaderContainer.emplace(shaderName, std::make_unique<Shader>(shaderPath_vs.c_str(), shaderPath_fs.c_str()));
	std::cout << "SUCCESS" << std::endl;
}

void ResourceManager::deleteShader(const std::string& shaderName) {
	if (shaderContainer.find(shaderName) == shaderContainer.end()) {
		std::cerr << "ERROR::ERROR SHADER " << shaderName << " DOES NOT EXIST IN THIS SCENE" << std::endl;
	}
	shaderContainer.erase(shaderName);
}

void ResourceManager::updateShader(const std::string& shaderPath_vs, const std::string& shaderPath_fs, const std::string& shaderName) {
	shaderContainer[shaderName].release();
	shaderContainer[shaderName] = std::make_unique<Shader>(shaderPath_vs.c_str(), shaderPath_fs.c_str());
}

// Test if contains

bool ResourceManager::hasMesh(const std::string& meshName) {
	if (meshContainer.find(meshName) != meshContainer.end()) {
		return true;
	}
	else { return false; }
}
bool ResourceManager::hasShader(const std::string& shaderName) {
	if (shaderContainer.find(shaderName) != shaderContainer.end()) {
		return true;
	}
	else { return false; }
}

bool ResourceManager::hasParticles(const std::string& containerName) {
	if (particleCollectionContainer.find(containerName) != particleCollectionContainer.end()) {
		return true;
	}
	else { return false; }
}

// Get Materials

Shader& ResourceManager::getShaderData(const std::string& shaderName) {
	if (shaderContainer.find(shaderName) == shaderContainer.end()) {
		std::cerr << "ERROR::ERROR SHADER " << shaderName << " DOES NOT EXIST IN THIS SCENE" << std::endl;
	}
	return *shaderContainer.find(shaderName)->second;
}

Mesh& ResourceManager::getMeshData(const std::string& meshName) {
	if (meshContainer.find(meshName) == meshContainer.end()) {
		std::cerr << "ERROR::ERROR MESH " << meshName << " DOES NOT EXIST IN THIS SCENE" << std::endl;
	}
	return meshContainer.find(meshName)->second;
}

std::vector<Vertex>& ResourceManager::getParticleData(const std::string& collectionName) {
	if (particleCollectionContainer.find(collectionName) == particleCollectionContainer.end()) {
		std::cerr << "ERROR:ERROR PARTICLE COLLECTION " << collectionName << " DOES NOT EXIST IN THIS SCENE" << std::endl;
	}
	return particleCollectionContainer.find(collectionName)->second;
}
