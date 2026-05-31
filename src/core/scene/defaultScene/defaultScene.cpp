#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>

#include "defaultScene.h"
#include "scene.h"

defaultScene::~defaultScene() {}

void defaultScene::init() 
{
	_Camera->Position = df_sSettings.defaultCamPos;

	glGenVertexArrays(1, &_gridVAO);

	_RM->loadResources();
	_NRenderer->setResources(_RM.get());

	//_MRenderer->updateGPU(_RM->getMeshData("cube").vertices, _RM->getMeshData("cube").indices);

	terrSettings tSet;
	df_terrain = std::make_unique<terrain>(tSet);
	df_terrain->buildTerrain();

	auto [verts, indices] = df_terrain->toMeshData();
	Mesh terrainMesh;
	terrainMesh.name = "terrain";
	terrainMesh.vertices = verts;
	terrainMesh.indices = indices;
	_RM->meshes().addMesh(terrainMesh, "terrain");
	terrainMeshID = _RM->meshes().uploadMesh("terrain");
}

void defaultScene::update(float dt, const Window& window)
{
	/*
	We need to get this to
	player.update(dt);
	*/
	_rotation += 50.0f * dt;
}

void defaultScene::render(const Window& window)
{
	/*
	* We need to get this down to something simple like
	* renderer.begin();
	* 
	* renderer.drawWorld();
	* renderer.drawTerrain();
	* renderer.drawDebug();
	* 
	* renderer.end();
	*/
	_NRenderer->beginFrame();

	_Camera->setPerspective((float)window.getWidth() / (float)window.getHeight());
	
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), _Camera->getPerspective(), 0.1f, 10000.0f);
	_NRenderer->setViewProj(_Camera->GetViewMatrix(), projection);
	_NRenderer->setViewPos(_Camera->Position);

	lightSource l1;
	l1.color = glm::vec3(1.0);
	l1.position = { 0.0f, 600.0f, 200.0f };
	l1.strength = 1.0f;

	lightSource l2;
	l2.color = glm::vec3(0.5f, 1.0f, 0.2f);
	l2.position = { 100.0f, 300.0f, 50.0f };
	l2.strength = 1.0f;

	lightSource l3;
	l3.color = glm::vec3(1.0f, 0.2f, 0.5f);
	l3.position = { 50.0f, 25.0f, 25.0f };
	l3.strength = 1.0f;

	_NRenderer->submitLight(l1);
	_NRenderer->submitLight(l2);
	_NRenderer->submitLight(l3);

	renderObject cubeObject;
	
	cubeObject.meshID = _RM->meshes().meshIDfromName("cube");
	cubeObject.shaderID = _RM->shaders().shaderIDfromName("blinnPhong");
	cubeObject.worldTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 150.0f, 0.0f));
	cubeObject.worldTransform *= glm::rotate(glm::mat4(1.0f), glm::radians(_rotation), glm::vec3(1.0,1.0,0.5));
	cubeObject.worldTransform *= glm::scale(glm::mat4(1.0f), glm::vec3(50.0f));
	cubeObject.material.ambient = glm::vec3(0.2f);
	cubeObject.material.diffuse = glm::vec3(0.8f);
	cubeObject.material.specular = glm::vec3(0.5f);
	cubeObject.material.shininess = 32.0f;
	_NRenderer->submitItem(cubeObject);

	renderObject monkeyObject;

	monkeyObject.meshID = _RM->meshes().meshIDfromName("monkey");
	monkeyObject.shaderID = _RM->shaders().shaderIDfromName("blinnPhong");
	monkeyObject.worldTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 400.0f, 0.0f));
	monkeyObject.worldTransform *= glm::rotate(glm::mat4(1.0f), glm::radians(_rotation), glm::vec3(1.0, 1.0, 0.5));
	monkeyObject.worldTransform *= glm::scale(glm::mat4(1.0f), glm::vec3(35.0f));
	monkeyObject.material.ambient = glm::vec3(0.2f);
	monkeyObject.material.diffuse = glm::vec3(0.8f);
	monkeyObject.material.specular = glm::vec3(0.5f);
	monkeyObject.material.shininess = 32.0f;
	_NRenderer->submitItem(monkeyObject);

	renderObject terrainObject;
	terrainObject.state.wireframe = false;
	terrainObject.meshID = terrainMeshID;
	terrainObject.shaderID = _RM->shaders().shaderIDfromName("defaultTerrain");
	terrainObject.worldTransform = glm::mat4(1.0f);
	terrainObject.material.ambient = glm::vec3(0.5f);
	terrainObject.material.diffuse = glm::vec3(0.8f);
	terrainObject.material.specular = glm::vec3(0.5f);
	terrainObject.material.shininess = 32.0f;
	_NRenderer->submitItem(terrainObject);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);

	uint32_t gridID = _RM->shaders().shaderIDfromName("debugGrid");
	Shader& gridShader = _RM->shaders().getShaderData(gridID);	gridShader.use();
	gridShader.setMat4("view", _Camera->GetViewMatrix());
	gridShader.setMat4("projection", projection);
	gridShader.setVec3("camPos", _Camera->Position);

	glBindVertexArray(_gridVAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);

	_NRenderer->flush();

	_NRenderer->endFrame();
}

void defaultScene::inputHandler(Window& window, float dt)
{
	GLFWwindow* glfwWindow = window.getWindow();

	if (Input::KeyDown(GLFW_KEY_W)) _Camera->ProcessInputs(FORWARD, dt);
	if (Input::KeyDown(GLFW_KEY_S)) _Camera->ProcessInputs(BACKWARD, dt);
	if (Input::KeyDown(GLFW_KEY_A)) _Camera->ProcessInputs(LEFT, dt);
	if (Input::KeyDown(GLFW_KEY_D)) _Camera->ProcessInputs(RIGHT, dt);
	if (Input::KeyDown(GLFW_KEY_SPACE)) _Camera->ProcessInputs(UP, dt);
	if (Input::KeyDown(GLFW_KEY_LEFT_CONTROL)) _Camera->ProcessInputs(DOWN, dt);
	if (Input::KeyDown(GLFW_KEY_ESCAPE)) glfwSetWindowShouldClose(glfwWindow, true);

	if (Input::KeyPressed(GLFW_KEY_TAB)) {
		if (df_sSettings.meshFill == true) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			df_sSettings.meshFill = false;
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			df_sSettings.meshFill = true;
		}
	}

	if (Input::KeyDown(GLFW_KEY_LEFT_SHIFT)) _Camera->CameraSpeed = 300.0f;
	else { _Camera->CameraSpeed = 50.0f; }
}
