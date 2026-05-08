#ifndef RIGIDBODYSYSTEM_H
#define RIGIDBODYSYSTEM_H

#include <vector>

#include "mesh.h"
#include "rigidBody.h"
#include "softBody.h"

struct settings
{
	float particleMass = 0.01f;
	float gravity = -5.5f;
	float springConst = 5.0f;
	float dampening = 0.3f;
	float angularDampening = 8.0f;
	float restitution = 0.5f;
	float stickiness = 0.9f;
};

enum class rbSysInputTypes
{
	PARTICLE_MASS,
	GRAVITY,
	SPR_CONST,
	SPR_DAMP,
	ANG_DAMP,
	COLLISION_RESTITUTION,
	STICKINESS,
};

class RigidBodySystem
{
public:
	void update(std::vector<rigidBody>& rBodies, std::vector<spring>& springs, const float& dt);
	void handleInputs(rbSysInputTypes input, float amount, std::vector<rigidBody>& rBodies);

private:
	void applyRigidBodyForces(std::vector<rigidBody>& rBodies, std::vector<spring>& springs);
	void applySpringForces(std::vector<rigidBody>& rBodies, std::vector<spring>& springs);
	void integrate(std::vector<rigidBody>& rBodies, std::vector<spring>& springs, const float& dt);
	void leapFrog(std::vector<rigidBody>& rBodies, std::vector<spring>& springs, const float& dt);
	void handleCollisions(std::vector<rigidBody>& rBodies, const float& dt);

	settings rbsSet;
};

#endif // !RIGIDBODYSYSTEM_H
