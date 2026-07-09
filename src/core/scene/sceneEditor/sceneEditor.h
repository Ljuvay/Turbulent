#ifndef SCENEEDITOR_H
#define SCENEEDITOR_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <string>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class sceneEditor
{
public:
	virtual void editorInit() = 0;

	virtual void editorBegin() = 0;
	virtual void editorEnd() = 0;

	virtual void renderElements() = 0;
private:
	virtual void defaultElement() = 0;
};

#endif // !SCENEEDITOR_H
