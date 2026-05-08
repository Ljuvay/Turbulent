#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include <vector>
#include <iostream>

#include "rigidBodySystem.h"
#include "mesh.h"
#include "rigidBody.h"
#include "softBody.h"

void RigidBodySystem::update(std::vector<rigidBody>& rBodies, std::vector<spring>& springs, const float& dt)
{
	const int steps = 4;
	for (int i = 0; i < steps; i++) {
		integrate(rBodies, springs, dt / steps);
		handleCollisions(rBodies, dt / steps);
	}
}

void RigidBodySystem::handleInputs(rbSysInputTypes input, float amount, std::vector<rigidBody>& rBodies) {
	switch (input) {
	case rbSysInputTypes::PARTICLE_MASS:
		if (amount < 0 && rbsSet.particleMass <= 0.01) {
			std::cout << "particle Mass would be too low" << std::endl;
			return;
		}
		rbsSet.particleMass += amount;
		for (rigidBody& rb : rBodies) {
			rb.totMass = 0.0f;
			for (rigidBodyNode& rbN : rb.rNodes) {
				rbN.mass = rbsSet.particleMass;
				rb.totMass += rbN.mass;
			}
		}
		std::cout << "Particle mass is now: " << rbsSet.particleMass << std::endl;
		break;
	case rbSysInputTypes::GRAVITY:
		rbsSet.gravity += amount;
		std::cout << "Gravity is now: " << rbsSet.gravity << std::endl;
		break;
	case rbSysInputTypes::SPR_CONST:
		rbsSet.springConst += amount;
		std::cout << "Spring Constant is now: " << rbsSet.springConst << std::endl;
		break;
	case rbSysInputTypes::SPR_DAMP:
		rbsSet.dampening += amount;
		std::cout << "Spring Dampening is now: " << rbsSet.dampening << std::endl;
		break;
	case rbSysInputTypes::ANG_DAMP:
		rbsSet.angularDampening += amount;
		std::cout << "Angular Spring Dampening is now: " << rbsSet.angularDampening << std::endl;
		break;
	case rbSysInputTypes::COLLISION_RESTITUTION:
		rbsSet.restitution += amount;
		std::cout << "Collision Restitution is now: " << rbsSet.restitution << std::endl;
		break;

	case rbSysInputTypes::STICKINESS:
		rbsSet.stickiness += amount;
		std::cout << "Stickiness is now: " << rbsSet.stickiness << std::endl;
		break;
	}
}

void RigidBodySystem::applyRigidBodyForces(std::vector<rigidBody>& rBodies, std::vector<spring>& springs)
{
	for (rigidBody& rb : rBodies) {
		rb.posForces = rb.totMass * glm::vec3(0.0f, rbsSet.gravity, 0.0f);
		rb.torque = -rbsSet.angularDampening * rb.angVelocity;
		rb.angVelocity = glm::clamp(rb.angVelocity, glm::vec3(-5.0f), glm::vec3(5.0f));
		rb.angMomentum = glm::clamp(rb.angMomentum, glm::vec3(-2.5f), glm::vec3(2.5f));
		//rb.posVelocity = glm::clamp(rb.posVelocity, glm::vec3(-5.0f), glm::vec3(5.0f));
	}

	for (rigidBody& rb : rBodies)
	{
		for (rigidBodyNode& n : rb.rNodes)
		{
			glm::vec3 nodeForce = rb.totMass * glm::vec3(0.0f);
			rb.posForces += nodeForce;
			glm::vec3 r = rb.rMatrix * n.leverArm;
			rb.torque += glm::cross(r, nodeForce);
		}
	}

}

void RigidBodySystem::applySpringForces(std::vector<rigidBody>& rBodies, std::vector<spring>& springs)
{
	for (spring& s : springs)
	{
		glm::vec3 deltaDist = (s.rbAnc->CenterOfMass + (s.rbAnc->rMatrix * s.vertAnc->leverArm)) -
			(s.rbPlat->CenterOfMass + (s.rbPlat->rMatrix * s.vertRB->leverArm));
		s.currLength = glm::length(deltaDist);
		if (s.currLength < 0.000001) { continue; }

		float invLength = 1.0f / s.currLength;
		glm::vec3 dir = deltaDist * invLength;

		float springForce = rbsSet.springConst * (s.currLength - s.restLength); // Replace .79 with spring const var
		springForce = glm::clamp(springForce, -50.0f, 50.0f);

		// Apply impuse to rb based off spring force
		s.rbAnc->posForces += -springForce * dir;
		s.rbPlat->posForces += springForce * dir;
		glm::vec3 rArm = s.rbPlat->rMatrix * s.vertRB->leverArm;
		s.rbPlat->torque += glm::cross(rArm, springForce * dir);
		s.rbAnc->torque += glm::cross(s.rbAnc->rMatrix * s.vertAnc->leverArm, -springForce * dir);


		glm::vec3 vA = s.rbAnc->posVelocity + glm::cross(s.rbAnc->angVelocity, s.rbAnc->rMatrix * s.vertAnc->leverArm);
		glm::vec3 vB = s.rbPlat->posVelocity + glm::cross(s.rbPlat->angVelocity, s.rbPlat->rMatrix * s.vertRB->leverArm);
		glm::vec3 relAngVelocity = vA - vB;

		float dampMagnitude = rbsSet.dampening * glm::dot(relAngVelocity, dir);
		glm::vec3 dampForce = dampMagnitude * dir;

		s.rbAnc->posForces += -dampForce;
		s.rbPlat->posForces += dampForce;
		s.rbPlat->torque += glm::cross(rArm, dampForce);
		s.rbAnc->torque += glm::cross(s.rbAnc->rMatrix * s.vertAnc->leverArm, -dampForce);
	}
}

void RigidBodySystem::integrate(std::vector<rigidBody>& rBodies, std::vector<spring>& springs, const float& dt)
{
	double a = 1.0f / (4.0f - std::pow(4.0, 1.0 / 3.0));
	double b = 1.0 - 4.0 * a;

	leapFrog(rBodies, springs, dt * a);
	leapFrog(rBodies, springs, dt * a);
	leapFrog(rBodies, springs, dt * b);
	leapFrog(rBodies, springs, dt * a);
	leapFrog(rBodies, springs, dt * a);
}

void RigidBodySystem::leapFrog(std::vector<rigidBody>& rBodies, std::vector<spring>& springs, const float& dt)
{
	applyRigidBodyForces(rBodies, springs);
	applySpringForces(rBodies, springs);

	for (rigidBody& rb : rBodies)
	{
		if (!rb.pinned) {
			rb.CenterOfMass += 0.5f * rb.posVelocity * dt;

			if (glm::length(rb.angVelocity) * dt > 0.000001) {
				glm::quat tempQuaternion;
				tempQuaternion = glm::angleAxis(glm::length(rb.angVelocity) * dt, glm::normalize(rb.angVelocity));
				rb.rMatrix = rb.rMatrix * glm::mat3_cast(tempQuaternion);
			}
		}
	}

	applyRigidBodyForces(rBodies, springs);
	applySpringForces(rBodies, springs);

	for(rigidBody& rb : rBodies){
		if(!rb.pinned) {
			rb.angMomentum += rb.torque * dt;
			rb.posVelocity += (rb.posForces / rb.totMass) * dt;
			rb.angVelocity = glm::inverse(rb.rMatrix * rb.MomOfInertia * glm::transpose(rb.rMatrix)) * rb.angMomentum;
		}
	}

	for (rigidBody& rb : rBodies) {
		if (!rb.pinned) {
			rb.CenterOfMass += 0.5f * rb.posVelocity * dt;
			rb.angVelocity *= 0.995;
			rb.angMomentum *= 0.995;
		}
	}
}

// Kinda a mess, but I implement the particle to triangle collision test from the slides
// as used in my softbody dynamics project. Bisection still needs to be added as when there are multiple hits
// per frame it has the tendancy to phase through.
//
// Update: still a mess, dont have time to fix, causes phasing through the platform ):
void RigidBodySystem::handleCollisions(std::vector<rigidBody>& rBodies, const float& dt)
{
	for (int i = 0; i < rBodies.size(); i++) {
		if (!rBodies[i].collidable) { continue; }

		// Get world positions of nodes
		std::vector<glm::vec3> worldPosVec;
		for (rigidBodyNode& rbN : rBodies[i].rNodes)
		{
			glm::vec3 pos = rBodies[i].getWorldPos(rbN);
			worldPosVec.push_back(pos);
		}

		// Gen triangles
		std::vector<triangle> iTriangles;
		for (int tIter = 0; tIter < rBodies[i].rInds.size(); tIter += 3)
		{
			triangle tTri;
			tTri.p0 = worldPosVec[rBodies[i].rInds[tIter]];
			tTri.p1 = worldPosVec[rBodies[i].rInds[tIter + 1]];
			tTri.p2 = worldPosVec[rBodies[i].rInds[tIter + 2]];
			tTri.normal = glm::normalize(glm::cross(tTri.p1 - tTri.p0, tTri.p2 - tTri.p0));
			iTriangles.push_back(tTri);
		}

		for (int j = i + 1; j < rBodies.size(); j++) {
			if (!rBodies[j].collidable) { continue; }

			// Get world position of nodes of other rigid body
			std::vector<glm::vec3> worldPosVecComp;
			for (rigidBodyNode& rbN : rBodies[j].rNodes)
			{
				glm::vec3 pos = rBodies[j].getWorldPos(rbN);
				worldPosVecComp.push_back(pos);
			}

			// Gen triangle for comparison
			std::vector<triangle> jTriangles;
			for (int tIter = 0; tIter < rBodies[j].rInds.size(); tIter += 3)
			{
				triangle tTri;
				tTri.p0 = worldPosVecComp[rBodies[j].rInds[tIter]];
				tTri.p1 = worldPosVecComp[rBodies[j].rInds[tIter + 1]];
				tTri.p2 = worldPosVecComp[rBodies[j].rInds[tIter + 2]];
				tTri.normal = glm::normalize(glm::cross(tTri.p1 - tTri.p0, tTri.p2 - tTri.p0));
				jTriangles.push_back(tTri);
			}

			// First test i's particles against j's triangles
			for (int pIterI = 0; pIterI < worldPosVec.size(); pIterI++)
			{
				glm::vec3 x0 = worldPosVec[pIterI];
				glm::vec3 r = rBodies[i].rMatrix * rBodies[i].rNodes[pIterI].leverArm;
				glm::vec3 v0 = rBodies[i].posVelocity + glm::cross(rBodies[i].angVelocity, r);
				float testDt = dt * 5.0f;

				/*
				for (triangle& t : jTriangles) {
					std::cout << "normal: " << t.normal.x << " " << t.normal.y << " " << t.normal.z << std::endl;
				}*/

				for (const triangle& t : jTriangles)
				{
					glm::vec3 e1 = t.p1 - t.p0;
					glm::vec3 e2 = t.p2 - t.p0;

					glm::vec3 tcros = glm::cross(e1, e2);

					float d = glm::dot(v0, t.normal);
					if (abs(d) < 0.000001) { continue; }

					float hitT = glm::dot(t.p0 - x0, t.normal) / d;
					if (hitT < 0.0f || hitT > testDt) 
					{
						//std::cout << "hitT < 0.0f or hitT > testDt" << std::endl;
						continue; 
					}

					glm::vec3 P = x0 + v0 * hitT;

					float u = (glm::dot(tcros, (glm::cross((P - t.p0), e2))) / glm::dot(tcros, tcros));
					float v = (glm::dot(tcros, (glm::cross(e1, (P - t.p0)))) / glm::dot(tcros, tcros));

					if (u >= 0 && u <= 1 && v >= 0 && v <= 1 && u + v <= 1) {
						// Particle is inside triangle
						//std::cout << "Particle inside triangle" << std::endl;

						float tLow = 0.0f;
						float tHigh = hitT;
						for (int timeIter = 0; timeIter < 32; timeIter++)
						{
							float tMid = (tLow + tHigh) * 0.5f;
							glm::vec3 xMid = x0 + v0 * tMid;
							if (glm::dot(xMid - t.p0, t.normal) < 0) { tHigh = tMid; }
							else tLow = tMid;
						}
						hitT = tLow;
						P = x0 + v0 * hitT;

						glm::vec3 vRel = rBodies[i].posVelocity - rBodies[j].posVelocity;
						float vn = glm::dot(vRel, t.normal);
						if (vn > 0.0f) { continue; }

						glm::vec3 rI = rBodies[i].rMatrix * rBodies[i].rNodes[pIterI].leverArm;
						glm::mat3 I = rBodies[i].rMatrix * rBodies[i].MomOfInertia * glm::transpose(rBodies[i].rMatrix);
						glm::vec3 nCross = glm::cross(rI, t.normal);

						float numerator = -(1.0f + rbsSet.restitution) * vn;
						float denominator = (1.0f / rBodies[i].totMass) + glm::dot(t.normal, glm::cross(glm::inverse(I) * nCross, rI));
						float A = numerator / denominator;

						rBodies[i].posVelocity += A * t.normal;
						rBodies[i].angMomentum += glm::cross(rI, A * t.normal);
						rBodies[i].angVelocity = glm::inverse(I) * rBodies[i].angMomentum;

						glm::vec3 rJ = rBodies[j].rMatrix * rBodies[j].getClosestNode(P).leverArm;
						glm::mat3 IJ = rBodies[j].rMatrix * rBodies[j].MomOfInertia * glm::transpose(rBodies[j].rMatrix);
						glm::vec3 nCrossJ = glm::cross(rJ, t.normal);

						float numeratorJ = -(1.0f + rbsSet.restitution) * glm::dot(rBodies[j].posVelocity + glm::cross(rBodies[j].angVelocity, rJ) - (rBodies[i].posVelocity + glm::cross(rBodies[i].angVelocity, rI)), t.normal);
						float denominatorJ = (1.0f / rBodies[j].totMass) + glm::dot(t.normal, glm::cross(glm::inverse(IJ) * nCrossJ, rJ));
						float AJ = numeratorJ / denominatorJ;

						rBodies[j].posVelocity -= AJ * t.normal;
						rBodies[j].angMomentum += glm::cross(rJ, -AJ * t.normal);
						rBodies[j].angVelocity = glm::inverse(IJ) * rBodies[j].angMomentum;

						x0 = P + t.normal * 0.1f + rBodies[i].posVelocity * hitT;
						rBodies[i].CenterOfMass = x0 - (rBodies[i].rMatrix * rBodies[i].rNodes[pIterI].leverArm);
						//break;
					}
				}
			}

			// Next test j's particles against i's triangles
			for (int pIterJ = 0; pIterJ < worldPosVecComp.size(); pIterJ++)
			{
				glm::vec3 x0 = worldPosVecComp[pIterJ];
				glm::vec3 r = rBodies[j].rMatrix * rBodies[j].rNodes[pIterJ].leverArm;
				glm::vec3 v0 = rBodies[j].posVelocity + glm::cross(rBodies[j].angVelocity, r);
				float testDt = dt * 5.0f;

				for (const triangle& t : iTriangles)
				{
					glm::vec3 e1 = t.p1 - t.p0;
					glm::vec3 e2 = t.p2 - t.p0;

					glm::vec3 tcros = glm::cross(e1, e2);

					float d = glm::dot(v0, t.normal);
					if (abs(d) < 0.000001) { continue; }

					float hitT = glm::dot(t.p0 - x0, t.normal) / d;
					if (hitT < 0.0f || hitT > testDt)
					{
						//std::cout << "hitT < 0.0f or hitT > testDt" << std::endl;
						continue;
					}
					glm::vec3 P = x0 + v0 * hitT;

					float u = (glm::dot(tcros, (glm::cross((P - t.p0), e2))) / glm::dot(tcros, tcros));
					float v = (glm::dot(tcros, (glm::cross(e1, (P - t.p0)))) / glm::dot(tcros, tcros));

					if (u >= 0 && u <= 1 && v >= 0 && v <= 1 && u + v <= 1) {
						// Particle is inside triangle
						//std::cout << "Particle inside triangle" << std::endl;

						float tLow = 0.0f;
						float tHigh = hitT;
						for (int timeIter = 0; timeIter < 32; timeIter++)
						{
							float tMid = (tLow + tHigh) * 0.5f;
							glm::vec3 xMid = x0 + v0 * tMid;
							if (glm::dot(xMid - t.p0, t.normal) < 0) { tHigh = tMid; }
							else tLow = tMid;
						}
						hitT = tLow;
						P = x0 + v0 * hitT;

						glm::vec3 vRel = rBodies[j].posVelocity - rBodies[i].posVelocity;
						float vn = glm::dot(vRel, t.normal);
						if (vn > 0.0f) { continue; }

						glm::vec3 rJ = rBodies[j].rMatrix * rBodies[j].rNodes[pIterJ].leverArm;
						glm::mat3 I = rBodies[j].rMatrix * rBodies[j].MomOfInertia * glm::transpose(rBodies[j].rMatrix);
						glm::vec3 nCross = glm::cross(rJ, t.normal);

						float numerator = -(1.0f + rbsSet.restitution) * vn;
						float denominator = (1.0f / rBodies[j].totMass) + glm::dot(t.normal, glm::cross(glm::inverse(I) * nCross, rJ));
						float A = numerator / denominator;

						rBodies[j].posVelocity += A * t.normal;
						rBodies[j].angMomentum += glm::cross(rJ, A * t.normal);
						rBodies[j].angVelocity = glm::inverse(I) * rBodies[j].angMomentum;

						glm::vec3 rI = rBodies[i].rMatrix * rBodies[i].getClosestNode(P).leverArm;
						glm::mat3 II = rBodies[i].rMatrix * rBodies[i].MomOfInertia * glm::transpose(rBodies[i].rMatrix);
						glm::vec3 nCrossI = glm::cross(rI, t.normal);

						float numeratorI = -(1.0f + rbsSet.restitution) * glm::dot(rBodies[i].posVelocity + glm::cross(rBodies[i].angVelocity, rI) - (rBodies[j].posVelocity + glm::cross(rBodies[j].angVelocity, rJ)), t.normal);
						float denominatorI = (1.0f / rBodies[i].totMass) + glm::dot(t.normal, glm::cross(glm::inverse(II) * nCrossI, rI));
						float AI = numeratorI / denominatorI;

						rBodies[i].posVelocity -= AI * t.normal;
						rBodies[i].angMomentum += glm::cross(rI, -AI * t.normal);
						rBodies[i].angVelocity = glm::inverse(II) * rBodies[i].angMomentum;

						x0 = P + t.normal * 0.1f + rBodies[j].posVelocity * hitT;
						rBodies[j].CenterOfMass = x0 - (rBodies[j].rMatrix * rBodies[j].rNodes[pIterJ].leverArm);
						//break;
					}
				}
			}
		}
	}
}