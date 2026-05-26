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

	_RM->addMesh("resources/models/cube.obj", "defaultCube");
	_RM->addShader("resources/shaders/defaultMesh_vs.glsl", "resources/shaders/defaultMesh_fs.glsl", "defaultShader");
	_RM->addShader("resources/shaders/debugGridTest_vs.glsl", "resources/shaders/debugGridTest_fs.glsl", "debugGridShader");
	_RM->addShader("resources/shaders/terrain_vs.glsl", "resources/shaders/terrain_fs.glsl", "terrainShader");

	std::vector<Vertex>& verts = _RM->getMeshData("defaultCube").vertices;
	for (Vertex& vtx : verts)
	{
		vtx.color = { 1.0f, 0.5f, 0.0f };
	}
	_MRenderer->updateGPU(_RM->getMeshData("defaultCube").vertices, _RM->getMeshData("defaultCube").indices);

	terrSettings tSet;
	df_terrain = std::make_unique<terrain>(tSet);
	df_terrain->buildTerrain();

	auto& chunks = df_terrain->getChunks();
	auto& chunkVerts = chunks[0]->getVertices();
	auto& chunkIndices = chunks[0]->getIndices();

	std::vector<Vertex> tempVerts;
	std::vector<GLuint> tempIndices;
	GLuint vertOffset = 0;
	
	for (auto& chunk : df_terrain->getChunks())
	{
		for (auto& tv : chunk->getVertices())
		{
			Vertex v;
			v.position = tv.pos;
			v.normal = tv.norm;
			v.uv = tv.uv;
			v.color = { 0.0f, 1.0f, 1.0f };
			tempVerts.push_back(v);
		}
		for (auto& idx : chunk->getIndices())
		{
			tempIndices.push_back(idx + vertOffset);
		}
		vertOffset += chunk->getVertices().size();
	}

	_TerrainRenderer = new meshRenderer();

	//std::vector<GLuint> tempIndices(chunkIndices.begin(), chunkIndices.end());
	_TerrainRenderer->updateGPU(tempVerts, tempIndices);

	std::cout << "Chunk count: " << chunks.size() << std::endl;
	std::cout << "Vert count: " << chunks[0]->getVertices().size() << std::endl;
	std::cout << "Index count: " << chunks[0]->getIndices().size() << std::endl;

	float minY = FLT_MAX, maxY = -FLT_MAX;
	for (auto& chunk : df_terrain->getChunks())
		for (auto& v : chunk->getVertices())
		{
			minY = std::min(minY, v.pos.y);
			maxY = std::max(maxY, v.pos.y);
		}
	std::cout << "MinY: " << minY << " MaxY: " << maxY << std::endl;
}

void defaultScene::update(float dt, const Window& window)
{
	_rotation += 50.0f * dt;
}

void defaultScene::render(const Window& window)
{
	_Camera->setPerspective((float)window.getWidth() / (float)window.getHeight());
	
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), _Camera->getPerspective(), 0.1f, 10000.0f);
	glm::mat4 view = _Camera->GetViewMatrix();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(_rotation), glm::vec3(-1.0f, 1.0f, -1.0f));

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	Shader& debugGridShader = _RM->getShaderData("debugGridShader");
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

	Shader& defaultShader = _RM->getShaderData("defaultShader");
	defaultShader.use();
	defaultShader.setMat4("view", view);
	defaultShader.setMat4("model", model);
	defaultShader.setMat4("projection", projection);
	_MRenderer->drawMesh();

	Shader& terrainShader = _RM->getShaderData("terrainShader");
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

	if (glfwGetKey(glfwWindow, GLFW_KEY_UP) == GLFW_PRESS) _Camera->ProcessInputs(FORWARD, dt);
	if (glfwGetKey(glfwWindow, GLFW_KEY_DOWN) == GLFW_PRESS) _Camera->ProcessInputs(BACKWARD, dt);
	if (glfwGetKey(glfwWindow, GLFW_KEY_LEFT) == GLFW_PRESS) _Camera->ProcessInputs(LEFT, dt);
	if (glfwGetKey(glfwWindow, GLFW_KEY_RIGHT) == GLFW_PRESS) _Camera->ProcessInputs(RIGHT, dt);
	if (glfwGetKey(glfwWindow, GLFW_KEY_SPACE) == GLFW_PRESS) _Camera->ProcessInputs(UP, dt);
	if (glfwGetKey(glfwWindow, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) _Camera->ProcessInputs(DOWN, dt);
	if (glfwGetKey(glfwWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(glfwWindow, true);
	if (glfwGetKey(glfwWindow, GLFW_KEY_T) == GLFW_PRESS) _Camera->resetRotation();

	if (glfwGetKey(glfwWindow, GLFW_KEY_TAB) == GLFW_PRESS && inputLastFrame == false) {
		if (df_sSettings.meshFill == true) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			df_sSettings.meshFill = false;
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			df_sSettings.meshFill = true;
		}
		inputLastFrame = true;
	}


	if (glfwGetKey(glfwWindow, GLFW_KEY_TAB) != GLFW_PRESS) { inputLastFrame = false; }
}
