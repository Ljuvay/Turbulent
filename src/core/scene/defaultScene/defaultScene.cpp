#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

	m_lights.push_back(l1);
	m_lights.push_back(l2);
	m_lights.push_back(l3);

	renderObject cubeObject;
	cubeObject.meshID = _RM->meshes().meshIDfromName("cube");
	cubeObject.shaderID = _RM->shaders().shaderIDfromName("blinnPhong");
	cubeObject.worldTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 150.0f, 0.0f));
	cubeObject.worldTransform *= glm::rotate(glm::mat4(1.0f), glm::radians(_rotation), glm::vec3(1.0, 1.0, 0.5));
	cubeObject.worldTransform *= glm::scale(glm::mat4(1.0f), glm::vec3(50.0f));
	cubeObject.material.ambient = glm::vec3(0.2f);
	cubeObject.material.diffuse = glm::vec3(0.8f);
	cubeObject.material.specular = glm::vec3(0.5f);
	cubeObject.material.shininess = 32.0f;

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

	renderObject terrainObject;
	terrainObject.state.wireframe = false;
	terrainObject.meshID = terrainMeshID;
	terrainObject.shaderID = _RM->shaders().shaderIDfromName("defaultTerrain");
	terrainObject.worldTransform = glm::mat4(1.0f);
	terrainObject.material.ambient = glm::vec3(0.5f);
	terrainObject.material.diffuse = glm::vec3(0.8f);
	terrainObject.material.specular = glm::vec3(0.5f);
	terrainObject.material.shininess = 32.0f;

	m_objects.push_back(cubeObject);
	m_objects.push_back(monkeyObject);
	m_objects.push_back(terrainObject);
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

	for (lightSource& ls : m_lights)
	{
		_NRenderer->submitLight(ls);
	}

	for (renderObject& ro : m_objects)
	{
		if (ro.meshID == _RM->meshes().meshIDfromName("monkey"))
		{
		ro.worldTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 400.0f, 0.0f));
		ro.worldTransform *= glm::rotate(glm::mat4(1.0f), glm::radians(_rotation), glm::vec3(1.0, 1.0, 0.5));
		ro.worldTransform *= glm::scale(glm::mat4(1.0f), glm::vec3(35.0f));
		}
		if (ro.meshID == _RM->meshes().meshIDfromName("cube"))
		{
		ro.worldTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 150.0f, 0.0f));
		ro.worldTransform *= glm::rotate(glm::mat4(1.0f), glm::radians(_rotation), glm::vec3(1.0, 1.0, 0.5));
		ro.worldTransform *= glm::scale(glm::mat4(1.0f), glm::vec3(50.0f));

		}

		_NRenderer->submitItem(ro);
	}

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

	if (!m_editorMode) {
		if (Input::KeyDown(GLFW_KEY_W)) _Camera->ProcessInputs(FORWARD, dt);
		if (Input::KeyDown(GLFW_KEY_S)) _Camera->ProcessInputs(BACKWARD, dt);
		if (Input::KeyDown(GLFW_KEY_A)) _Camera->ProcessInputs(LEFT, dt);
		if (Input::KeyDown(GLFW_KEY_D)) _Camera->ProcessInputs(RIGHT, dt);
		if (Input::KeyDown(GLFW_KEY_SPACE)) _Camera->ProcessInputs(UP, dt);
		if (Input::KeyDown(GLFW_KEY_LEFT_CONTROL)) _Camera->ProcessInputs(DOWN, dt);
		if (Input::KeyDown(GLFW_KEY_ESCAPE)) glfwSetWindowShouldClose(glfwWindow, true);
	}

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

	if (Input::KeyPressed(GLFW_KEY_F1)) {
		glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		m_editorMode = true;
	}
	if (Input::KeyPressed(GLFW_KEY_F2)) {
		glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		m_editorMode = false;
	}

	if (Input::KeyDown(GLFW_KEY_LEFT_SHIFT)) _Camera->CameraSpeed = 300.0f;
	else { _Camera->CameraSpeed = 50.0f; }

	window.editorMode = m_editorMode;
}

void defaultScene::onImGui()
{
	ImGui::Begin("Engine stats");

	ImGui::Text("FPS: %.1f", fps);
	ImGui::Text("Frame Time: %.2f ms", frameTime);

	ImGui::Separator();

	ImGui::Text("Camera Position");
	ImGui::Text("%.2f %.2f %.2f",
		_Camera->Position.x,
		_Camera->Position.y,
		_Camera->Position.z);

	ImGui::Separator();

	if (ImGui::CollapsingHeader("Lights"))
	{
		ImGui::Indent();
		for (size_t i = 0; i < m_lights.size(); i++) {
			if (ImGui::CollapsingHeader(("Light [" + std::to_string(i) + "]").c_str()))
			{
				ImGui::ColorEdit3(("Color##" + std::to_string(i)).c_str(), glm::value_ptr(m_lights[i].color));
				ImGui::SliderFloat(("Strength##" + std::to_string(i)).c_str(), &m_lights[i].strength, 0.0, 1.0);
				ImGui::DragFloat3(("Position##" + std::to_string(i)).c_str(), glm::value_ptr(m_lights[i].position), 1.0, -1000.0f, 1000.0f);
			}
		}
		ImGui::Unindent();
	}

	if (ImGui::CollapsingHeader("Objects"))
	{
		ImGui::Indent();
		for (size_t i = 0; i < m_objects.size(); i++) {
			if (ImGui::CollapsingHeader(("Objects [" + std::to_string(i) + "]").c_str()))
			{
				ImGui::Text("Transform");
				ImGui::Separator();
				ImGui::Text("Material");
				ImGui::ColorEdit3(("Diffuse Color##" + std::to_string(i)).c_str(), glm::value_ptr(m_objects[i].material.diffuse));
				ImGui::ColorEdit3(("Specular Color##" + std::to_string(i)).c_str(), glm::value_ptr(m_objects[i].material.specular));
				ImGui::SliderFloat(("Shininess##" + std::to_string(i)).c_str(), &m_objects[i].material.shininess, 0.0, 1.0);
				ImGui::Checkbox(("Ambient##" + std::to_string(i)).c_str(), &m_objects[i].material.useAmbient);
				ImGui::BeginDisabled(!m_objects[i].material.useAmbient);
				ImGui::ColorEdit3(("Almbient Color##" + std::to_string(i)).c_str(), glm::value_ptr(m_objects[i].material.ambient));
				ImGui::EndDisabled();
				ImGui::Separator();
				ImGui::Text("GLState");
			}
		}
		ImGui::Unindent();
	}

	ImGui::Separator();

	ImGui::End();
}

/* Im Gui mini cheat sheet 

text
("Text %.1f", value);

button
if(ImGui::Button("button name") { action(); }

checkbox
ImGui::CheckBox("checkbox name", &bool);

slider
ImGui::SliderFloat("slider name", &sliderVariable, minVal, maxVal);
ImGui::SliderInt("slider name", &sliderVariable, minVal, maxVal);

inputbox
ImGui::InputFloat("input name", &inputVariable);
ImGui::InputInt("input name", &inputVariable);

colorpicker needs <glm/gtc/type_ptr.hpp
ImGui::ColorEdit3("object name", glm::value_ptr(objectColor);

Trees
if (ImGui::TreeNode("Tree name")
{
	ImGui::SliderFloat(...);
	ImGui::TreePop();
}

collapsing headers
if (ImGui::CollapsingHeader("header name")
{
	...
}

combo boxes
const char* items[] ]
{
	"item",
	"item"
};

ImGui::Combo("Combo name", &object slection, items, IM_ARRAYSIZE(items));

seperator
ImGui::Seperator(); // Just draws a line

*/