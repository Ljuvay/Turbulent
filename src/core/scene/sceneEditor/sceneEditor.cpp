#include "sceneEditor.h"
#include "resourceManager.h"
#include <glm/gtc/type_ptr.hpp>
#include <string>

void sceneEditor::render(Camera& camera, forwardRenderer& renderer, ResourceManager& rm,
    std::vector<instancedLight>& lights, std::vector<instancedModel>& objects,
    float fps, float frameTime, bool editorMode)
{
    renderStatsPanel(camera, renderer, fps, frameTime, editorMode);
    renderLightsPanel(lights);
    renderObjectsPanel(objects);
    renderSpawnPanel(rm, lights, objects);
}

void sceneEditor::renderStatsPanel(Camera& camera, forwardRenderer& renderer, float fps, float frameTime, bool editorMode)
{
    ImGui::Begin("Engine stats");

    ImGui::Text("FPS: %.1f", fps);
    ImGui::Text("Frame Time: %.2f ms", frameTime);
    ImGui::Text("Mode: %s", editorMode ? "Editor" : "Play");

    ImGui::Separator();

    ImGui::Text("Camera Position");
    ImGui::Text("%.2f %.2f %.2f",
        camera.Position.x,
        camera.Position.y,
        camera.Position.z);

    ImGui::Text("Background Color");
    ImGui::ColorEdit3("BackGroundColor##", glm::value_ptr(renderer.clearColor));
    ImGui::DragFloat("Camera FOV##", &camera.Fov, 1.0f, 30.0f, 90.0f);
    ImGui::DragFloat("Camera View Distance##", &camera.maxView, 250.0f, 500.0f, 100000.0f);

    ImGui::Separator();

    ImGui::End();
}

void sceneEditor::renderLightsPanel(std::vector<instancedLight>& lights)
{
    ImGui::Begin("Engine stats");

    if (!ImGui::CollapsingHeader("Lights")) { ImGui::End(); return; }

    ImGui::Indent();
    for (size_t i = 0; i < lights.size(); i++) {
        auto& light = lights[i];
        if (ImGui::CollapsingHeader(("Light [" + std::to_string(i) + "]").c_str()))
        {
            ImGui::ColorEdit3(("Color##" + std::to_string(i)).c_str(), glm::value_ptr(light.lt.color));
            ImGui::DragFloat(("Strength##" + std::to_string(i)).c_str(), &light.lt.strength, 0.1f, 0.0f, 128.0f);
            ImGui::DragFloat3(("Position##" + std::to_string(i)).c_str(), glm::value_ptr(light.lt.position), 1.0f, -1000.0f, 1000.0f);
            ImGui::DragFloat3(("Direction##" + std::to_string(i)).c_str(), glm::value_ptr(light.lt.direction), 0.5f, -1000.0f, 1000.0f);
            ImGui::Checkbox(("Draw Light##" + std::to_string(i)).c_str(), &light.lt.draw);
            ImGui::Checkbox(("Casts Shadows##" + std::to_string(i)).c_str(), &light.lt.castsShadows);
            if (ImGui::Button(("Delete##" + std::to_string(i)).c_str())) {
                lights.erase(lights.begin() + i);
                break;
            }
        }
    }
    ImGui::Unindent();

    ImGui::End();
}

void sceneEditor::renderObjectsPanel(std::vector<instancedModel>& objects)
{
    ImGui::Begin("Engine stats");

    if (!ImGui::CollapsingHeader("Objects")) { ImGui::End(); return; }

    ImGui::Indent();
    for (size_t i = 0; i < objects.size(); i++) {
        auto& inst = objects[i];
        if (ImGui::CollapsingHeader((inst.name + "##" + std::to_string(i)).c_str()))
        {
            ImGui::Text("Transform");
            ImGui::DragFloat3(("Translation##" + std::to_string(i)).c_str(), glm::value_ptr(inst.obj.worldTransform.translation), 1.0f, -1000.0f, 1000.0f);
            ImGui::DragFloat3(("Rotation##" + std::to_string(i)).c_str(), glm::value_ptr(inst.obj.worldTransform.rotation), 1.0f, -1000.0f, 1000.0f);
            ImGui::DragFloat3(("Scale##" + std::to_string(i)).c_str(), glm::value_ptr(inst.obj.worldTransform.scale), 1.0f, 0.0f, 1000.0f);

            float uniformScale = inst.obj.worldTransform.scale.x;
            if (ImGui::DragFloat(("Uniform Scale##" + std::to_string(i)).c_str(), &uniformScale, 1.0f, 0.0f, 1000.0f))
                inst.obj.worldTransform.scale = glm::vec3(uniformScale);

            ImGui::Separator();
            ImGui::Text("Material");
            ImGui::ColorEdit3(("Diffuse Color##" + std::to_string(i)).c_str(), glm::value_ptr(inst.obj.material.diffuse));
            ImGui::ColorEdit3(("Specular Color##" + std::to_string(i)).c_str(), glm::value_ptr(inst.obj.material.specular));
            ImGui::SliderFloat(("Shininess##" + std::to_string(i)).c_str(), &inst.obj.material.shininess, 0.0f, 64.0f);
            ImGui::Checkbox(("Ambient##" + std::to_string(i)).c_str(), &inst.obj.material.useAmbient);
            ImGui::BeginDisabled(!inst.obj.material.useAmbient);
            ImGui::ColorEdit3(("Ambient Color##" + std::to_string(i)).c_str(), glm::value_ptr(inst.obj.material.ambient));
            ImGui::EndDisabled();

            ImGui::Separator();
            ImGui::Text("GLState");
            ImGui::Checkbox(("Wire Frame##" + std::to_string(i)).c_str(), &inst.obj.state.wireframe);
            ImGui::Checkbox(("Depth Test##" + std::to_string(i)).c_str(), &inst.obj.state.depthTest);
            ImGui::Checkbox(("Depth Mask##" + std::to_string(i)).c_str(), &inst.obj.state.depthMask);
            ImGui::Checkbox(("Blend##" + std::to_string(i)).c_str(), &inst.obj.state.blend);
            ImGui::Checkbox(("Cull Face##" + std::to_string(i)).c_str(), &inst.obj.state.cullFace);

            if (ImGui::Button(("Delete##" + std::to_string(i)).c_str())) {
                objects.erase(objects.begin() + i);
                break;
            }
        }
    }
    ImGui::Unindent();

    ImGui::End();
}

void sceneEditor::renderSpawnPanel(ResourceManager& rm, std::vector<instancedLight>& lights, std::vector<instancedModel>& objects)
{
    ImGui::Begin("Available Objects");

    ImGui::Text("Light");
    if (ImGui::Button("Create Light")) {
        lights.push_back(instancedLight{});
    }

    std::vector<std::string> meshNames = rm.meshes().getMeshNames();
    for (size_t i = 0; i < meshNames.size(); i++)
    {
        ImGui::Text(meshNames[i].c_str());
        if (ImGui::Button(("Create: " + meshNames[i]).c_str())) {
            instancedModel newObject;
            newObject.obj.meshID = rm.meshes().meshIDfromName(meshNames[i]);
            newObject.obj.shaderID = rm.shaders().shaderIDfromName("blinnPhong");
            newObject.name = meshNames[i];
            objects.push_back(newObject);
        }
    }

    ImGui::End();
}