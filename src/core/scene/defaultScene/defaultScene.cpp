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

	m_Renderer->flush();

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

void defaultScene::onImGui()
{
	ImGui::Begin("Engine stats");

	ImGui::Text("FPS: %.1f", fps);
	ImGui::Text("Frame Time: %.2f ms", frameTime);
	ImGui::Text("Mode: %s", m_editorMode ? "Editor" : "Play");

	ImGui::Separator();

	ImGui::Text("Camera Position");
	ImGui::Text("%.2f %.2f %.2f",
		_Camera->Position.x,
		_Camera->Position.y,
		_Camera->Position.z);

	ImGui::Text("Background Color");
	ImGui::ColorEdit3("BackGroundColor##", glm::value_ptr(m_Renderer->clearColor));
	ImGui::DragFloat(("Camera FOV##"), &_Camera->Fov, 1.0, 30.0, 90.0);
	ImGui::DragFloat(("Camera View Distance##"), &_Camera->maxView, 250.0, 500.0, 100000.0);

	ImGui::Separator();

	if (ImGui::CollapsingHeader("Lights"))
	{
		ImGui::Indent();
		for (size_t i = 0; i < m_lights.size(); i++) {
			if (ImGui::CollapsingHeader(("Light [" + std::to_string(i) + "]").c_str()))
			{
				ImGui::ColorEdit3(("Color##" + std::to_string(i)).c_str(), glm::value_ptr(m_lights[i].lt.color));
				ImGui::DragFloat(("Strength##" + std::to_string(i)).c_str(), &m_lights[i].lt.strength, 0.1, 0.0, 128.0);
				ImGui::DragFloat3(("Position##" + std::to_string(i)).c_str(), glm::value_ptr(m_lights[i].lt.position), 1.0, -1000.0f, 1000.0f);
				ImGui::Checkbox(("Draw Light##" + std::to_string(i)).c_str(), &m_lights[i].lt.draw);
				if (ImGui::Button(("Delete##" + std::to_string(i)).c_str())) {
					m_lights.erase(m_lights.begin() + i);
					break;
				}

			}
		}
		ImGui::Unindent();
	}

	std::vector<std::string> meshNames = m_RM->meshes().getMeshNames();
	if (ImGui::CollapsingHeader("Objects"))
	{
		ImGui::Indent();
		for (size_t i = 0; i < m_objects.size(); i++) {
			if (ImGui::CollapsingHeader((m_objects[i].name + "##" + std::to_string(i)).c_str()))
			{
				ImGui::Text("Transform");
				ImGui::DragFloat3(("Translation##" + std::to_string(i)).c_str(), glm::value_ptr(m_objects[i].obj.worldTransform.translation), 1.0, -1000.0f, 1000.0f);
				ImGui::DragFloat3(("Rotation##" + std::to_string(i)).c_str(), glm::value_ptr(m_objects[i].obj.worldTransform.rotation), 1.0, -1000.0f, 1000.0f);
				ImGui::DragFloat3(("Scale##" + std::to_string(i)).c_str(), glm::value_ptr(m_objects[i].obj.worldTransform.scale), 1.0, 0.0f, 1000.0f);
				float uniformScale = m_objects[i].obj.worldTransform.scale.x;
				if (ImGui::DragFloat(("Uniform Scale##" + std::to_string(i)).c_str(), &uniformScale, 1.0, 0.0f, 1000.0f))
				{
					m_objects[i].obj.worldTransform.scale = glm::vec3(uniformScale);
				}
				ImGui::Separator();
				ImGui::Text("Material");
				ImGui::ColorEdit3(("Diffuse Color##" + std::to_string(i)).c_str(), glm::value_ptr(m_objects[i].obj.material.diffuse));
				ImGui::ColorEdit3(("Specular Color##" + std::to_string(i)).c_str(), glm::value_ptr(m_objects[i].obj.material.specular));
				ImGui::SliderFloat(("Shininess##" + std::to_string(i)).c_str(), &m_objects[i].obj.material.shininess, 0.0, 64.0);
				ImGui::Checkbox(("Ambient##" + std::to_string(i)).c_str(), &m_objects[i].obj.material.useAmbient);
				ImGui::BeginDisabled(!m_objects[i].obj.material.useAmbient);
				ImGui::ColorEdit3(("Almbient Color##" + std::to_string(i)).c_str(), glm::value_ptr(m_objects[i].obj.material.ambient));
				ImGui::EndDisabled();
				ImGui::Separator();
				ImGui::Text("GLState");
				ImGui::Checkbox(("Wire Frame##" + std::to_string(i)).c_str(), &m_objects[i].obj.state.wireframe);
				ImGui::Checkbox(("Depth Test##" + std::to_string(i)).c_str(), &m_objects[i].obj.state.depthTest);
				ImGui::Checkbox(("Depth Mask##" + std::to_string(i)).c_str(), &m_objects[i].obj.state.depthMask);
				ImGui::Checkbox(("Blend##" + std::to_string(i)).c_str(), &m_objects[i].obj.state.blend);
				ImGui::Checkbox(("Cull Face##" + std::to_string(i)).c_str(), &m_objects[i].obj.state.cullFace);
				if (ImGui::Button(("Delete##" + std::to_string(i)).c_str())) {
					m_objects.erase(m_objects.begin() + i); 
					break;
				}
			}
		}
		ImGui::Unindent();
	}

	ImGui::Separator();

	ImGui::End();

	ImGui::Begin("Available Objects");
	ImGui::Text("Light");
	if (ImGui::Button("Create Light")) {
		instancedLight newLight;

		m_lights.push_back(newLight);
	}

	for (int i = 0; i < meshNames.size(); i++)
	{
		ImGui::Text(meshNames[i].c_str());
		if (ImGui::Button(("Create: " + meshNames[i]).c_str())) {
			instancedModel newObject;
			newObject.obj.meshID = m_RM->meshes().meshIDfromName(meshNames[i]);
			newObject.obj.shaderID = m_RM->shaders().shaderIDfromName("blinnPhong");
			newObject.name = meshNames[i];
			m_objects.push_back(newObject);
		}
	}
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