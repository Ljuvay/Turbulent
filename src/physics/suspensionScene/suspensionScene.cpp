#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <memory>
#include <vector>
#include <string>
#include <random>

#include "scene.h"
#include "suspensionScene.h"


suspensionScene::~suspensionScene() {}

void suspensionScene::init()
{
	rSys = std::make_unique<RigidBodySystem>();
	_Camera->Position = s_sceneSettings.defaultCamPos;

	if (!s_sceneSettings.meshFill) { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); }
	else { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); }

	initResources();
	initRigidBodies();
	initSprings();
}

void suspensionScene::update(float dt, const Window& window)
{
	if (!s_sceneSettings.paused)
	{
		rSys->update(rBodies, springs, dt);
		for (rigidBody& rb : rBodies) {
			rb.update(dt);
		}
	}
}

void suspensionScene::render(const Window& window) 
{
	_Camera->setPerspective((float)window.getWidth() / (float)window.getHeight());
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), _Camera->getPerspective(), 0.1f, 100.0f);
	glm::mat4 view = _Camera->GetViewMatrix();
	glm::mat4 model = glm::mat4(1.0f);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	Shader& bbShader = _RM->getShaderData("bbShader");
	bbShader.use();
	bbShader.setMat4("view", view);
	bbShader.setMat4("model", model);
	bbShader.setMat4("projection", projection);
	for (int i = 0; i < rBodies.size(); i++) {
		_MRenderer->updateGPU(rBodies[i].getRefMesh()->vertices, rBodies[i].getRefMesh()->indices);		
		_MRenderer->drawMesh();
	}
	
	std::vector<Vertex> springVerts;
	for (spring& s : springs)
	{
		Vertex v1, v2;
		v1.position = s.rbAnc->getWorldPos(*s.vertAnc);
		v2.position = s.rbPlat->getWorldPos(*s.vertRB);
		springVerts.push_back(v1);
		springVerts.push_back(v2);
	}
	_MRenderer->updateGPU(springVerts, {});
	_MRenderer->drawLines();
}

void suspensionScene::inputHandler(Window& window, float dt)
{
	GLFWwindow* glfwWindow = window.getWindow();

	// CAMERA & WINDOW CONTROLS
	if (glfwGetKey(glfwWindow, GLFW_KEY_UP) == GLFW_PRESS) _Camera->ProcessInputs(FORWARD, dt);
	if (glfwGetKey(glfwWindow, GLFW_KEY_DOWN) == GLFW_PRESS) _Camera->ProcessInputs(BACKWARD, dt);
	if (glfwGetKey(glfwWindow, GLFW_KEY_LEFT) == GLFW_PRESS) _Camera->ProcessInputs(LEFT, dt);
	if (glfwGetKey(glfwWindow, GLFW_KEY_RIGHT) == GLFW_PRESS) _Camera->ProcessInputs(RIGHT, dt);
	if (glfwGetKey(glfwWindow, GLFW_KEY_SPACE) == GLFW_PRESS) _Camera->ProcessInputs(UP, dt);
	if (glfwGetKey(glfwWindow, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) _Camera->ProcessInputs(DOWN, dt);
	if (glfwGetKey(glfwWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(glfwWindow, true);
	if (glfwGetKey(glfwWindow, GLFW_KEY_T) == GLFW_PRESS) _Camera->resetRotation();
	if (glfwGetKey(glfwWindow, GLFW_KEY_TAB) == GLFW_PRESS && pressedLastFrame == false) {
		if (s_sceneSettings.meshFill == true) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			s_sceneSettings.meshFill = false;
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			s_sceneSettings.meshFill = true;
		}
		pressedLastFrame = true;
	}

	//if (glfwGetKey(glfwWindow, GLFW_KEY_G) == GLFW_PRESS) rSys->handleInputs(true);
	//else { rSys->handleInputs(false); }

	// SIMULATION CONTROLS
	
	if (glfwGetKey(glfwWindow, GLFW_KEY_E) == GLFW_PRESS && pressedLastFrame == false) {
		if (!_RM->hasMesh("newCube")) {
			_RM->addMesh("resources/models/cube.obj", "newCube");
			rigidBody spawnedRB;
			spawnedRB.setMesh(_RM->getMeshData("newCube"));
			spawnedRB.initialize();
			spawnedRB.collidable = true;
			spawnedRB.pinned = false;
			spawnedRB.CenterOfMass = { 0.0f, 10.0f, 0.0f };
			rBodies.push_back(spawnedRB);
		}
		else {
			std::cout << "cannot summon more" << std::endl;
		}
		pressedLastFrame = true;
	}

	if (glfwGetKey(glfwWindow, GLFW_KEY_M) == GLFW_PRESS && pressedLastFrame == false) {
		float amt = 0.0f;
		if ((glfwGetKey(glfwWindow, GLFW_KEY_LEFT_SHIFT))) { amt = 0.01f; }
		else { amt = -0.01f; }
		rSys->handleInputs(rbSysInputTypes::PARTICLE_MASS, amt, rBodies);
		pressedLastFrame = true;
	}

	if (glfwGetKey(glfwWindow, GLFW_KEY_G) == GLFW_PRESS && pressedLastFrame == false) {
		float amt = 0.0f;
		if ((glfwGetKey(glfwWindow, GLFW_KEY_LEFT_SHIFT))) { amt = 0.5f; }
		else { amt = -0.5f; }
		rSys->handleInputs(rbSysInputTypes::GRAVITY, amt, rBodies);
		pressedLastFrame = true;
	}

	if (glfwGetKey(glfwWindow, GLFW_KEY_S) == GLFW_PRESS && pressedLastFrame == false) {
		float amt = 0.0f;
		if ((glfwGetKey(glfwWindow, GLFW_KEY_LEFT_SHIFT))) { amt = 0.5f; }
		else { amt = -0.5f; }
		rSys->handleInputs(rbSysInputTypes::SPR_CONST, amt, rBodies);
		pressedLastFrame = true;
	}

	if (glfwGetKey(glfwWindow, GLFW_KEY_D) == GLFW_PRESS && pressedLastFrame == false) {
		float amt = 0.0f;
		if ((glfwGetKey(glfwWindow, GLFW_KEY_LEFT_SHIFT))) { amt = 0.1f; }
		else { amt = -0.1f; }
		rSys->handleInputs(rbSysInputTypes::SPR_DAMP, amt, rBodies);
		pressedLastFrame = true;
	}

	if (glfwGetKey(glfwWindow, GLFW_KEY_A) == GLFW_PRESS && pressedLastFrame == false) {
		float amt = 0.0f;
		if ((glfwGetKey(glfwWindow, GLFW_KEY_LEFT_SHIFT))) { amt = 0.25f; }
		else { amt = -0.25f; }
		rSys->handleInputs(rbSysInputTypes::ANG_DAMP, amt, rBodies);
		pressedLastFrame = true;
	}

	if (glfwGetKey(glfwWindow, GLFW_KEY_Q) == GLFW_PRESS && pressedLastFrame == false) {
		float amt = 0.0f;
		if ((glfwGetKey(glfwWindow, GLFW_KEY_LEFT_SHIFT))) { amt = 0.05f; }
		else { amt = -0.05f; }
		rSys->handleInputs(rbSysInputTypes::COLLISION_RESTITUTION, amt, rBodies);
		pressedLastFrame = true;
	}

	if (glfwGetKey(glfwWindow, GLFW_KEY_X) == GLFW_PRESS && pressedLastFrame == false) {
		float amt = 0.0f;
		if ((glfwGetKey(glfwWindow, GLFW_KEY_LEFT_SHIFT))) { amt = 0.05f; }
		else { amt = -0.05f; }
		rSys->handleInputs(rbSysInputTypes::STICKINESS, amt, rBodies);
		pressedLastFrame = true;
	}

	if (glfwGetKey(glfwWindow, GLFW_KEY_R) == GLFW_PRESS && pressedLastFrame == false) {
		if (_RM->hasMesh("newCube")) {
			_RM->deleteMesh("newCube");
		}
		rBodies.clear();
		springs.clear();

		initRigidBodies();
		initSprings();
		pressedLastFrame = true;
	}

	if (glfwGetKey(glfwWindow, GLFW_KEY_P) == GLFW_PRESS && pressedLastFrame == false) {
		if (s_sceneSettings.paused == true) {
			s_sceneSettings.paused = false;
		}
		else {
			s_sceneSettings.paused = true;
		}
		pressedLastFrame = true;
	}

	// Cut random spring
	if (glfwGetKey(glfwWindow, GLFW_KEY_C) == GLFW_PRESS && pressedLastFrame == false) {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<int> dist(0, springs.size() - 1);

		if (springs.size() > 0) {
			springs.erase(springs.begin() + dist(gen));
		}
		for (spring& s : springs) {
			s.rbAnc->angMomentum *= 0.98;
			s.rbPlat->angMomentum *= 0.98; // To help fix crazy behavior when theres like 1-2 springs
			s.rbAnc->angVelocity *= 0.98;
			s.rbPlat->angVelocity *= 0.98;
			s.rbAnc->posVelocity *= 0.98;
			s.rbPlat->posVelocity *= 0.98;
		}
		std::cout << "A random spring has been cut!" << std::endl;
		pressedLastFrame = true;
	}

	if (glfwGetKey(glfwWindow, GLFW_KEY_E) != GLFW_PRESS &&
		glfwGetKey(glfwWindow, GLFW_KEY_R) != GLFW_PRESS &&
		glfwGetKey(glfwWindow, GLFW_KEY_P) != GLFW_PRESS &&
		glfwGetKey(glfwWindow, GLFW_KEY_C) != GLFW_PRESS &&
		glfwGetKey(glfwWindow, GLFW_KEY_M) != GLFW_PRESS &&
		glfwGetKey(glfwWindow, GLFW_KEY_G) != GLFW_PRESS &&
		glfwGetKey(glfwWindow, GLFW_KEY_S) != GLFW_PRESS &&
		glfwGetKey(glfwWindow, GLFW_KEY_D) != GLFW_PRESS &&
		glfwGetKey(glfwWindow, GLFW_KEY_A) != GLFW_PRESS &&
		glfwGetKey(glfwWindow, GLFW_KEY_Q) != GLFW_PRESS &&
		glfwGetKey(glfwWindow, GLFW_KEY_T) != GLFW_PRESS &&
		glfwGetKey(glfwWindow, GLFW_KEY_X) != GLFW_PRESS &&
		glfwGetKey(glfwWindow, GLFW_KEY_TAB) != GLFW_PRESS)
	{
		pressedLastFrame = false;
	}
}

// Private functions

void suspensionScene::initResources()
{
	_RM->addShader("resources/shaders/defaultMesh_vs.glsl", "resources/shaders/defaultMesh_fs.glsl", "bbShader");
	_RM->addMesh("resources/models/cube.obj", "mxmxSpringCube");
	_RM->addMesh("resources/models/cube.obj", "mxmnSpringCube");
	_RM->addMesh("resources/models/cube.obj", "mnmxSpringCube");
	_RM->addMesh("resources/models/cube.obj", "mnmnSpringCube");
	_RM->addMesh("resources/models/cube.obj", "rbPlatform");
}

void suspensionScene::initRigidBodies() 
{
	rigidBody mxmxSpringCube;
	mxmxSpringCube.setMesh(_RM->getMeshData("mxmxSpringCube"));
	mxmxSpringCube.initialize();
	rigidBody mxmnSpringCube;
	mxmnSpringCube.setMesh(_RM->getMeshData("mxmnSpringCube"));
	mxmnSpringCube.initialize();
	rigidBody mnmxSpringCube;
	mnmxSpringCube.setMesh(_RM->getMeshData("mnmxSpringCube"));
	mnmxSpringCube.initialize();
	rigidBody mnmnSpringCube;
	mnmnSpringCube.setMesh(_RM->getMeshData("mnmnSpringCube"));
	mnmnSpringCube.initialize();
	rigidBody rbPlatform;
	rbPlatform.setMesh(_RM->getMeshData("rbPlatform"));
	rbPlatform.initialize();

	mxmxSpringCube.CenterOfMass = { 10.0f, 5.0f, 5.0f };
	mxmnSpringCube.CenterOfMass = { 10.0f, 5.0f, -5.0f };
	mnmxSpringCube.CenterOfMass = { -10.0f, 5.0f, 5.0f };
	mnmnSpringCube.CenterOfMass = { -10.0f, 5.0f, -5.0f };

	mxmxSpringCube.pinned = true;
	mxmnSpringCube.pinned = true;
	mnmxSpringCube.pinned = true;
	mnmnSpringCube.pinned = true;
	rbPlatform.collidable = true;

	rBodies.push_back(mxmxSpringCube);
	rBodies.push_back(mxmnSpringCube);
	rBodies.push_back(mnmxSpringCube);
	rBodies.push_back(mnmnSpringCube);
	rBodies.push_back(rbPlatform);

	/*
	Mesh& temp = _RM->getMeshData("rbPlatform");
	for (int i = 0; i < temp.vertices.size(); i++) {
		temp.vertices[i].position.x *= 5.0;
		temp.vertices[i].position.y *= 0.5;
		temp.vertices[i].position.z *= 2.5;
	}*/
	
	for (rigidBodyNode& rbN : rBodies[4].rNodes)
	{
		rbN.leverArm.x *= 5.0f;
		rbN.leverArm.y *= 0.25f;
		rbN.leverArm.z *= 2.5f;
	}
}

void suspensionScene::initSprings()
{
	rigidBodyNode& mxmx = rBodies[0].getClosestNode();
	rigidBodyNode& mxmn = rBodies[1].getClosestNode();
	rigidBodyNode& mnmx = rBodies[2].getClosestNode();
	rigidBodyNode& mnmn = rBodies[3].getClosestNode();
	spring mxmxSpr{ &mxmx, &rBodies[4].getClosestNode(rBodies[0].CenterOfMass), &rBodies[0], &rBodies[4]};
	spring mxmnSpr{ &mxmn, &rBodies[4].getClosestNode(rBodies[1].CenterOfMass), &rBodies[1], &rBodies[4] };
	spring mnmxSpr{ &mnmx, &rBodies[4].getClosestNode(rBodies[2].CenterOfMass), &rBodies[2], &rBodies[4] };
	spring mnmnSpr{ &mnmn, &rBodies[4].getClosestNode(rBodies[3].CenterOfMass), &rBodies[3], &rBodies[4] };

	springs.push_back(mxmxSpr);
	springs.push_back(mxmnSpr);
	springs.push_back(mnmxSpr);
	springs.push_back(mnmnSpr);

	int rbCounter = 0;
	for (spring& s : springs)
	{
		s.restLength = glm::length(glm::abs(rBodies[rbCounter++].getWorldPos(*s.vertAnc) - rBodies[4].getWorldPos(*s.vertRB)));
	}
}
