#ifndef DEFAULTSCENE_H
#define DEFAULTSCENE_H

#include <glm/glm.hpp>

#include <iostream>

#include "scene.h"
#include "resourceManager.h"
#include "meshRenderer.h"
#include "shader.h"
#include "vertex.h"

struct sceneSettings
{
	glm::vec3 defaultCamPos = { 0.0f, 0.0f, 10.0f };
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

	sceneSettings df_sSettings;

	float _rotation = 0.0f;
	bool inputLastFrame = false;
};

#endif // !DEFAULTSCENE_H