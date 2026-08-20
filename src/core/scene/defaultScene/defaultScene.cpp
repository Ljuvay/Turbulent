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

	m_debugRenderer = std::make_unique<debugRenderer>();
	m_debugRenderer->init(m_RM);

	df_terrain = std::make_unique<terrain>();
	df_terrain->buildTerrain();

	auto [verts, indices] = df_terrain->toMeshData();
	Mesh terrainMesh;
	terrainMesh.name = "terrain";
	terrainMesh.vertices = verts;
	terrainMesh.indices = indices;
	m_RM->meshes().addMesh(terrainMesh, "terrain");
	terrainMeshID = m_RM->meshes().uploadMesh("terrain");
}

void defaultScene::update(float dt, const Window& window)
{
	/*
	We need to get this to
	player.update(dt);
	*/
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
	m_Renderer->beginFrame();

	_Camera->setPerspective((float)window.getWidth() / (float)window.getHeight());
	
	m_Renderer->setViewProj(_Camera->GetViewMatrix(), _Camera->getProjection());
	m_Renderer->setViewPos(_Camera->Position);

	for (instancedLight& ls : m_lights)
	{
		m_Renderer->submitLight(ls.lt);
	}

	for (instancedModel& ro : m_objects)
	{
		m_Renderer->submitItem(ro.obj);
	}

	m_debugRenderer->drawGrid(_Camera->GetViewMatrix(), _Camera->getProjection(), _Camera->Position);

	m_Renderer->flush(window.getWidth(), window.getHeight());

	m_Renderer->endFrame();
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

void defaultScene::onImGui() {}

void defaultScene::buildEditor(sceneEditor& editor)
{
	editor.render(*_Camera, *m_Renderer, *m_RM, m_lights, m_objects, fps, frameTime, m_editorMode);
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