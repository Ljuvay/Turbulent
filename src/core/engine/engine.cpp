// ENGINE ABSTRACTION

#include "engine.h"

constexpr int DEFAULT_WINDOW_WIDTH = 800;
constexpr int DEFAULT_WINDOW_HEIGHT = 600;

constexpr float FIXED_DT = 0.016f;

Engine::Engine() :
    m_EngineContext(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, "TEngine")
{}

void Engine::init()
{
	Input::init(m_EngineContext.m_window.getWindow());

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(m_EngineContext.m_window.getWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 330");

    m_EngineContext.m_resourceManager.loadResources();
    m_EngineContext.m_Renderer = std::make_unique<forwardRenderer>();
    m_EngineContext.m_Renderer->init();
    m_EngineContext.m_Renderer->setResources(&m_EngineContext.m_resourceManager);

    m_EngineContext.m_sceneManager.initDefaultScene();

    m_EngineContext.m_window.setScene(m_EngineContext.m_sceneManager.getScene());
    m_EngineContext.m_sceneManager.getScene()->setRenderer(*m_EngineContext.m_Renderer);
    m_EngineContext.m_sceneManager.getScene()->setResources(m_EngineContext.m_resourceManager);
    m_EngineContext.m_sceneManager.getScene()->init();
}

void Engine::run()
{
    float lastFrame = glfwGetTime();
    float frameAccumulator = 0.0f;

    while (!glfwWindowShouldClose(m_EngineContext.m_window.getWindow()))
    {
        float currentFrame = glfwGetTime();
        float frameTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        if (frameTime >= 0.05) { frameTime = 0.05f; }

        frameAccumulator += frameTime;
        if (frameAccumulator > FIXED_DT * 3) {
            frameAccumulator = FIXED_DT * 3;
        }

        m_EngineContext.m_window.beginFrame();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //ImGui::ShowDemoWindow();
        m_EngineContext.m_sceneManager.getScene()->fps = 1.0f / frameTime;
        m_EngineContext.m_sceneManager.getScene()->frameTime = frameTime * 1000.0f;
        m_EngineContext.m_sceneManager.getScene()->onImGui();

        while (frameAccumulator >= FIXED_DT) {
            m_EngineContext.m_sceneManager.getScene()->inputHandler(m_EngineContext.m_window, FIXED_DT);
            Input::Update();
            m_EngineContext.m_sceneManager.updateScene(FIXED_DT, m_EngineContext.m_window);
            frameAccumulator -= FIXED_DT;
        }

        m_EngineContext.m_sceneManager.renderScene(m_EngineContext.m_window);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        m_EngineContext.m_window.endFrame();
    }
}

void Engine::shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
}