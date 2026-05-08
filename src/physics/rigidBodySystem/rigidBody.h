#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <memory>

#include "mesh.h"
#include "vertex.h"

struct rigidBodyNode
{
	glm::vec3 leverArm = glm::vec3(0.0f);
	glm::vec3 color = glm::vec3(0.0f);
	float mass = 0.0f;
};

struct triangle
{
	glm::vec3 p0, p1, p2;
	glm::vec3 normal;
};

class rigidBody
{
public:
	std::vector<rigidBodyNode> rNodes = {};
	std::vector<unsigned int> rInds = {};
	glm::vec3 CenterOfMass = glm::vec3(0.0f);
	glm::vec3 posVelocity = glm::vec3(0.0f);
	glm::vec3 angVelocity = glm::vec3(0.0f);
	glm::vec3 posForces = glm::vec3(0.0f);
	glm::vec3 torque = glm::vec3(0.0f);
	glm::mat3 rMatrix = glm::mat3(1.0f);
	glm::vec3 angMomentum = glm::vec3(0.0f);
	glm::mat3 MomOfInertia = glm::mat3(0.0f);
	glm::mat3 invMomOfInertia = glm::mat3(0.0f);
	float totMass = 0.0f;
	bool pinned = false;
	bool collidable = false;

	void initialize();
	void setMesh(const Mesh& inMesh);
	void update(const float& dt);
	void applyForce(const glm::vec3& force, const glm::vec3& loc);
	std::shared_ptr<Mesh> getRefMesh();
	glm::vec3 getWorldPos(rigidBodyNode& rbN);
	rigidBodyNode& getClosestNode(glm::vec3 loc = glm::vec3(0.0f));
	rigidBodyNode& getClosestNode(rigidBodyNode& rbNode);

private:
	void calculateComponents();
	void updateMesh();
	std::shared_ptr<Mesh> referenceMesh;
};

#endif // !RIGIDBODY_H
