#include <string>
#include <iostream>
#include <unordered_map>

#include "resourceManager.h"
#include "fileLoader.h"
#include "mesh.h"
#include "resourceTargets.h"

// Constructa
ResourceManager::ResourceManager()
{
	rm_meshSystem = std::make_unique<meshSystem>();
	rm_shaderSystem = std::make_unique<shaderSystem>();
}

// Load Resources

void ResourceManager::loadResources()
{
	loadMeshGroup();
	loadParticleGroup();
	loadShaderGroup();
}

// Particle
/*
void ResourceManager::createEmptyParticleGroup(const std::string& collectionName) {
	std::cout << "Trying to create empty Particle Group: \"" << collectionName << "\" to scene | STATUS: ";
	if (particleContainer.find(collectionName) != particleContainer.end()) {
		std::cerr << "FAILURE \"" << collectionName << "\" ALREADY EXIST" << std::endl;
	}
	else 
	{ 
	std::vector<Vertex> particles;
	particleContainer.emplace(collectionName, particles);
	std::cout << "SUCCESS" << std::endl; 
	}
}

void ResourceManager::addParticles(const std::vector<Vertex>& particles, const std::string& collectionName)
{
	std::cout << "Trying to add Particle: \"" << collectionName << "\" to scene | STATUS: ";
	if (particleContainer.find(collectionName) != particleContainer.end())
	{
		std::cerr << "FAILURE \"" << collectionName << "\" ALREADY EXIST" << std::endl;
	}
	else
	{
		particleCollectionContainer.emplace(collectionName, particles);
		std::cout << "SUCCESS" << std::endl;
	}
}

void ResourceManager::deleteParticles(const std::string& collectionName) {
	if (particleContainer.find(collectionName) == particleContainer.end()) {
		std::cerr << "ERROR:ERROR PARTICLE COLLECTION " << collectionName << " DOES NOT EXIST IN THIS SCENE" << std::endl;
	}
}

void ResourceManager::updateParticles(const std::vector<Vertex>& particles, const std::string& collectionName) {
	particleContainer[collectionName] = particles;
}

bool ResourceManager::hasParticles(const std::string& containerName) {
	if (particleContainer.find(containerName) != particleContainer.end()) {
		return true;
	}
	else { return false; }
}

std::vector<Vertex>& ResourceManager::getParticleData(const std::string& collectionName) {
	if (particleCollectionContainer.find(collectionName) == particleCollectionContainer.end()) {
		std::cerr << "ERROR:ERROR PARTICLE COLLECTION " << collectionName << " DOES NOT EXIST IN THIS SCENE" << std::endl;
	}
	return particleCollectionContainer.find(collectionName)->second;
}
*/

// Private Loaders

void ResourceManager::loadMeshGroup()
{
	rm_meshSystem->loadMeshGroup();
	/*for (const auto& t : MESH_TARGETS)
	{
		rm_meshSystem->addMesh(std::string(t.path), std::string(t.name));
	}
	*/
}

void ResourceManager::loadParticleGroup()
{
	return; // idk if ill need this for awhile
}

void ResourceManager::loadShaderGroup()
{
	rm_shaderSystem->loadShaderGroup();
	/*for (const auto& t : SHADER_TARGETS)
	{
		rm_shaderSystem->addShader(std::string(t.vs), std::string(t.fs), std::string(t.name));
	}
	*/
}
