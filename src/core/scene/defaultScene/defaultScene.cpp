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
	terrainSystem.update(camera.position);
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

	renderObject cubeObject;
	
	cubeObject.meshID = _RM->meshes().meshIDfromName("cube");
	cubeObject.shaderID = _RM->shaders().shaderIDfromName("defaultMesh");
	cubeObject.worldTransform = glm::mat4(1.0f);
	_NRenderer->submitItem(cubeObject);

	renderObject terrainObject;
	terrainObject.meshID = terrainMeshID;
	terrainObject.shaderID = _RM->shaders().shaderIDfromName("defaultTerrain");
	terrainObject.worldTransform = glm::mat4(1.0f);
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
}
