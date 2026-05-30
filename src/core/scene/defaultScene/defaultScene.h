#ifndef DEFAULTSCENE_H
#define DEFAULTSCENE_H

#include <glm/glm.hpp>

#include <iostream>

#include "scene.h"
#include "resourceManager.h"
#include "forwardRenderer.h"
#include "shader.h"
#include "vertex.h"
#include "terrain.h"
#include "terrSettings.h"
#include "input.h"

struct sceneSettings
{
	glm::vec3 defaultCamPos = { 0.0f, 5.0f, 0.0f };
	bool meshFill = true;
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

private:
	void initResources();

	std::unique_ptr<terrain> df_terrain;
	sceneSettings df_sSettings;

	unsigned int _gridVAO = 0;
	float _rotation = 0.0f;

	// Temp b4 refactor
	uint32_t terrainMeshID = 0;
};

#endif // !DEFAULTSCENE_H