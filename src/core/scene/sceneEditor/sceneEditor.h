#ifndef SCENEEDITOR_H
#define SCENEEDITOR_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <string>

#include "sceneObjects.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "camera.h"
#include "forwardRenderer.h"

struct instancedLight;
struct instancedModel;
class resourceManager;

class sceneEditor
{
public:
	sceneEditor() = default;

	void render(Camera& camera, forwardRenderer& renderer, ResourceManager& rm, 
		std::vector<instancedLight>& lights, std::vector<instancedModel>& objects,
		float fps, float frametime, bool editorMode);

private:
	void renderStatsPanel(Camera& camera, forwardRenderer& renderer, float fps, float freameTime, bool editorMode);
	void renderLightsPanel(std::vector<instancedLight>& lights);
	void renderObjectsPanel(std::vector<instancedModel>& objects);
	void renderSpawnPanel(ResourceManager& rm, std::vector<instancedLight>& lights, std::vector<instancedModel>& objects);

	const Camera* m_camera = nullptr;
	forwardRenderer* m_renderer = nullptr;
	ResourceManager* m_RM = nullptr;
	std::vector<instancedLight>* m_lights = nullptr;
	std::vector<instancedModel>* m_objects = nullptr;
};

#endif // !SCENEEDITOR_H
