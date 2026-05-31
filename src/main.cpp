#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "shader.h"

#include "input.h"
#include "sceneManager.h"
#include "window.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

const int DEFAULT_WINDOW_WIDTH = 800;
const int DEFAULT_WINDOW_HEIGHT = 600;

const float FIXED_DT = 0.001f;

int main() {
    Window window(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);

    Input::init(window.getWindow());

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window.getWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 330");

    sceneManager m_sceneManager;
    m_sceneManager.initDefaultScene();

    window.setScene(m_sceneManager.getScene());

    float lastFrame = glfwGetTime();
    float frameAccumulator = 0.0f;

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Minimal main loop
    while (!glfwWindowShouldClose(window.getWindow())) 
    {
        float currentFrame = glfwGetTime();
        float frameTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        if (frameTime >= 0.05) { frameTime = 0.05f; }

        frameAccumulator += frameTime;
        if (frameAccumulator > FIXED_DT * 3) {
            frameAccumulator = FIXED_DT * 3;
        }

        window.beginFrame();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //ImGui::ShowDemoWindow();
        m_sceneManager.getScene()->fps = 1.0f / frameTime;
        m_sceneManager.getScene()->frameTime = frameTime * 1000.0f;
        m_sceneManager.getScene()->onImGui();

        while (frameAccumulator >= FIXED_DT) {
            m_sceneManager.getScene()->inputHandler(window, FIXED_DT);
            Input::Update();
            m_sceneManager.updateScene(FIXED_DT, window);
            frameAccumulator -= FIXED_DT;
        }

        m_sceneManager.renderScene(window);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        window.endFrame();
    }

    //delete scene;
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}
