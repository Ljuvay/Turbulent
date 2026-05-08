#ifndef SUSPENSIONSCENE_H
#define SUSPENSIONSCENE_H

#include <glm/glm.hpp>

#include "scene.h"
#include "resourceManager.h"
#include "particleRenderer.h"
#include "meshRenderer.h"
#include "rigidBody.h"
#include "softBody.h"
#include "rigidBodySystem.h"

struct sceneSettings
{
	glm::vec3 defaultCamPos = { 0.0f,7.5f,25.0f };
	bool meshFill = true;
	bool paused = false;
};

class suspensionScene : public Scene
{
public:
	~suspensionScene() override;

	void init() override;
	void update(float dt, const Window& window) override;
	void render(const Window& window) override;
	void inputHandler(Window& window, float dt) override;

private:
	void initResources();
	void initRigidBodies();
	void initSprings();

	std::vector<rigidBody> rBodies;
	std::vector<spring> springs;

	std::unique_ptr<RigidBodySystem> rSys;

	sceneSettings s_sceneSettings;
};

#endif // !SUSPENSIONSCENE_H
