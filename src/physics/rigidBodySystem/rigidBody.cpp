#include <glm/glm.hpp>

#include <vector>
#include <memory>
#include <iostream>
#include <limits>
#include <random>

#include "rigidBody.h"

void rigidBody::initialize()
{
	if (referenceMesh == nullptr)
	{
		std::cout << "ERROR INITIALIZING RIGIDBODY: NEEDS REFERENCE MESH" << std::endl;
		return;
	}

	calculateComponents();
}

void rigidBody::setMesh(const Mesh& inMesh)
{
	referenceMesh = std::make_shared<Mesh>(inMesh);
	//std::cout << "Set Mesh address = " << &inMesh << std::endl;
	//std::cout << "Ref Mesh address = " << &referenceMesh << std::endl;
	//std::cout << "Rigid Body Mesh Set Successfully" << std::endl;
}

void rigidBody::update(const float& dt)
{
	updateMesh();
}

void rigidBody::applyForce(const glm::vec3& force, const glm::vec3& loc)
{

}

std::shared_ptr<Mesh> rigidBody::getRefMesh()
{
	//std::cout << "Returning refMesh address: " << &referenceMesh << std::endl;
	return referenceMesh;
}

glm::vec3 rigidBody::getWorldPos(rigidBodyNode& rbN)
{
	return CenterOfMass + (rMatrix * rbN.leverArm);
}

rigidBodyNode& rigidBody::getClosestNode(glm::vec3 loc)
{
	float maxLimit = std::numeric_limits<float>::max();
	rigidBodyNode *returnNode;
	for (rigidBodyNode& rbN : rNodes) 
	{
		float dist = glm::length(getWorldPos(rbN) - loc);
		if (dist < maxLimit)
		{
			maxLimit = dist;
			returnNode = &rbN;
		}
	}
	return *returnNode;
}

rigidBodyNode& rigidBody::getClosestNode(rigidBodyNode& rbNode)
{
	float maxLimit = std::numeric_limits<float>::max();
	rigidBodyNode* returnNode;
	for (rigidBodyNode& rbN : rNodes)
	{
		if (&rbN == &rbNode) { continue; }
		float dist = glm::length(getWorldPos(rbN) - getWorldPos(rbNode));
		if (dist < maxLimit)
		{
			maxLimit = dist;
			returnNode = &rbN;
		}
	}
	return *returnNode;
}

void rigidBody::calculateComponents()
{
	totMass = 0.0f;
	MomOfInertia = glm::mat3(0.0f);
	torque = glm::vec3(0.0f);

	rNodes.resize(referenceMesh->vertices.size());
	//std::cout << "rNodes size: " << rNodes.size() << " refMesh vert size: " << referenceMesh->vertices.size() << std::endl;
	//std::cout << "check 1 " << std::endl;
	rNodes.reserve(referenceMesh->vertices.size());
	for (rigidBodyNode& n : rNodes) { n.mass = 0.1f; } // Add settings to remove magic numbers
	
	rInds.resize(referenceMesh->indices.size());
	for (int i = 0; i < referenceMesh->indices.size(); i++)
	{
		rInds[i] = referenceMesh->indices[i];
	}

	//std::cout << "check 2 " << std::endl;
	glm::vec3 CoM(0.0f);
	for (int i = 0; i < referenceMesh->vertices.size(); i++)
	{
		CoM += referenceMesh->vertices[i].position * 0.1f;
		totMass += 0.1;
	}
	CenterOfMass = CoM / totMass;

	//std::cout << "check 3 " << std::endl;
	for (int i = 0; i < referenceMesh->vertices.size(); i++)
	{
		rNodes[i].leverArm = referenceMesh->vertices[i].position - CenterOfMass;
	}

	//std::cout << "check 4 " << std::endl;
	for (int i = 0; i < rNodes.size(); i++)
	{
		//torque += glm::cross(rMatrix * rNodes[i].leverArm, torque);
		glm::mat3 Itemp = 0.1f * (glm::dot(rNodes[i].leverArm, rNodes[i].leverArm) * glm::mat3(1.0f) - glm::outerProduct(rNodes[i].leverArm, rNodes[i].leverArm));
		MomOfInertia += Itemp;
	}

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dist(0.0f, 1.0f);

	for (int i = 0; i < referenceMesh->vertices.size(); i++)
	{
		rNodes[i].color = { dist(gen), dist(gen), dist(gen) };
		//std::cout << rNodes[i].color.x << " " << rNodes[i].color.y << " " << rNodes[i].color.z << std::endl;
	
	}
	//std::cout << "Rigid Body components calculated successfully" << std::endl;
}

void rigidBody::updateMesh()
{
	for (int i = 0; i < referenceMesh->vertices.size(); i++)
	{
		referenceMesh->vertices[i].position = CenterOfMass + (rMatrix * rNodes[i].leverArm);
	}
}
