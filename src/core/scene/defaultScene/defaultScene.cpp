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

	_MRenderer->updateGPU(_RM->getMeshData("cube").vertices, _RM->getMeshData("cube").indices);

	terrSettings tSet;
	df_terrain = std::make_unique<terrain>(tSet);
	df_terrain->buildTerrain();

	_TerrainRenderer = new meshRenderer();

	auto [verts, indices] = df_terrain->toMeshData();
	_TerrainRenderer->updateGPU(verts, indices);
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


	_Camera->setPerspective((float)window.getWidth() / (float)window.getHeight());
	
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), _Camera->getPerspective(), 0.1f, 10000.0f);
	glm::mat4 view = _Camera->GetViewMatrix();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(_rotation), glm::vec3(-1.0f, 1.0f, -1.0f));

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	Shader& debugGridShader = _RM->getShaderData("debugGrid");
	debugGridShader.use();
	debugGridShader.setMat4("view", view);
	debugGridShader.setMat4("projection", projection);
	debugGridShader.setVec3("camPos", _Camera->Position);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_FALSE);

	glBindVertexArray(_gridVAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	if (df_sSettings.meshFill)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	Shader& defaultShader = _RM->getShaderData("defaultMesh");
	defaultShader.use();
	defaultShader.setMat4("view", view);
	defaultShader.setMat4("model", model);
	defaultShader.setMat4("projection", projection);
	_MRenderer->drawMesh();

	Shader& terrainShader = _RM->getShaderData("defaultTerrain");
	terrainShader.use();
	terrainShader.setMat4("view", view);
	terrainShader.setMat4("model", glm::mat4(1.0f));
	terrainShader.setMat4("projection", projection);
	terrSettings tSet;
	terrainShader.setFloat("maxTerrainHeight", tSet.terrHeight);
	_TerrainRenderer->drawMesh();
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
