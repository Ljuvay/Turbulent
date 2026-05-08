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

static void instructionsMsg() {
    std::cout << "\n\n\n\n\n\n\n\n" << std::endl;
    std::cout << "---RIGID BODY SUSPENSION SIMULATION---" << std::endl;
    std::cout << "By Dylan Kauffman\n" << std::endl;
    std::cout << "---Simulation Controls---" << std::endl;
    std::cout << "e - Summon rigid body box (Can only be one | Reset sim to try again)" << std::endl;
    std::cout << "c - Cut a random spring" << std::endl;
    std::cout << "m/M -/+ Particle Mass  (Default 0.01)" << std::endl;
    std::cout << "g/G -/+ Gravity (Default -5.0)" << std::endl;
    std::cout << "s/S -/+ Spring Constant (Default 5.0)" << std::endl;
    std::cout << "d/D -/+ Linear Spring Dampening (Default 0.3)" << std::endl;
    std::cout << "a/A -/+ Angular Spring Dampening (Default 8.0)" << std::endl;
    std::cout << "q/Q -/+ Collision Restitution (Default 0.5)" << std::endl;
    std::cout << "x/X -/+ Collision Stickiness (Default 0.9)" << std::endl;
    std::cout << "R - Reset Simulation" << std::endl;
    std::cout << "P - Pause Simulation" << std::endl;
    std::cout << "Esc - Exit" << std::endl << std::endl;
    std::cout << "---Camera Controls---" << std::endl;
    std::cout << "TAB - Toggile Wireframe mode" << std::endl;
    std::cout << "T - Reset Camera Rotation" << std::endl;
    std::cout << "UP/DOWN/LEFT/RIGHT Arrow - Move Camera forward/backward/left/right" << std::endl;
    std::cout << "SPACE/Ctrl - Move Camera up/down\n\n\n" << std::endl;
    std::cout << "\n\nI know its physically inaccurate with collisions, sadly I ran out of time ):" << std::endl;
}

int main() {
    Window window(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
    Scene* scene = new suspensionScene();
    scene->init();
    window.setScene(scene);

    float lastFrame = glfwGetTime();
    float frameAccumulator = 0.0f;

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    instructionsMsg();

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
