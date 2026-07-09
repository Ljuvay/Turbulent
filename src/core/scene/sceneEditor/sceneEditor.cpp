#include "sceneEditor.h"

editorUI::editorInit()
{

}

void sceneEditor::editorBegin()
{
	ImGui::Begin("Engine Stats");

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
}

void sceneEditor::editorEnd()
{
	ImGui::End();
}

void sceneEditor::renderElements()
{

}

void sceneEditor::defaultElement()
{

}