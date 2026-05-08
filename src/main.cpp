// Clemson CPSC 4190
// Project 5 - Flocking Simulation (:
//
// Dylan Kauffman

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "shader.h"

#include "scene.h"
#include "suspensionScene.h"
#include "window.h"

const int DEFAULT_WINDOW_WIDTH = 800;
const int DEFAULT_WINDOW_HEIGHT = 600;

const float FIXED_DT = 0.004f;

int main() {
    Window window(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
    Scene* scene = new suspensionScene();
    scene->init();
    window.setScene(scene);

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

        while (frameAccumulator >= FIXED_DT) {
            scene->inputHandler(window, FIXED_DT);
            scene->update(FIXED_DT, window);
            frameAccumulator -= FIXED_DT;
        }

        scene->render(window);

        window.endFrame();
    }

    //delete scene;
    glfwTerminate();
    return 0;
}
