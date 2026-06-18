#ifndef DEFAULTSCENE_H
#define DEFAULTSCENE_H

#include <glm/glm.hpp>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>

#include "scene.h"
#include "resourceManager.h"
#include "forwardRenderer.h"
#include "debugRenderer.h"
#include "shader.h"
#include "vertex.h"
#include "terrain.h"
#include "terrSettings.h"
#include "input.h"
#include "sceneObjects.h"

struct sceneSettings
{
	glm::vec3 defaultCamPos = { 0.0f, 5.0f, 0.0f };
	bool paused = false;
};

class defaultScene : public Scene
{
public:
	~defaultScene() override;

	void init() override;
	void update(float dt, const Window& window) override;
	void render(const Window& window) override;
	void inputHandler(Window& window, float dt) override;

	void onImGui();
private:
	std::vector<instancedLight> m_lights;
	std::vector<instancedModel> m_objects;

	std::unique_ptr<debugRenderer> m_debugRenderer;
	std::unique_ptr<terrain> df_terrain;
	sceneSettings df_sSettings;

	bool m_editorMode = false;
	float _rotation = 0.0f;

	// Temp b4 refactor
	uint32_t terrainMeshID = 0;
	uint32_t billBoardID = 0;
};

#endif // !DEFAULTSCENE_H