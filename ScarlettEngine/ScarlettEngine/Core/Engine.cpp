#include "ScarlettEnginePch.h"
#include "Engine.h"

#include "WindowLayer.h"
#include "Input/LayerStack.h"

#include "Core/Window/WindowManager.h"
#include "Rendering/Vulkan/VulkanRendererEditor.h"

#include <ScarlettGameCore/Src/GameCore.h>
#include <ScarlettGameCore/Components/Transform.h>
#include <ScarlettGameCore/Components/SquareSprite.h>

#include "Systems/SquareSpriteSystem.h"

namespace Scarlett
{

Engine* Engine::mInstance = nullptr;

void Engine::InitEngine()
{
    SCARLETT_ASSERT(mInstance == nullptr && "Engine has already been initialized, can only initialize engine once.");
    mInstance = this;

    Log::Init();

    mLayerStack = new LayerStack();

    std::string windowConfiguration;
#if defined(SCARLETT_DEBUG)
    windowConfiguration = " - Dev";
#elif defined(SCARLETT_RELEASE)
    windowConfiguration = " - Release";
#else
#error No valid configuration to add to window title.
#endif

    WindowManager::InitApi();

    WindowProperties winProps;
    winProps.title += windowConfiguration;

    mMainWindow = WindowManager::CreateWindowInternal(winProps);
    mWindowLayer = mLayerStack->PushLayer<WindowLayer>();

    mMainWindow->SetEventCallback(SCARLETT_BIND_FUNCTION(Engine::OnEvent));

    ScarlettGame::GameCore::Instance().Init();

    Renderer::Instance().Init(mMainWindow);

    SCARLETT_DLOG("Engine Initialized");

    SquareSpriteSystemProperties properties;
    properties.device = Renderer::Instance().GetDevice();
    ScarlettGame::GameCore::Instance().GetActiveScene()->RegisterSystem<SquareSpriteSystem>(&properties);

    auto square1 = ScarlettGame::GameCore::Instance().CreateEntity();
    square1.GetComponent<ScarlettGame::Transform>()->translation = glm::vec3(0.5f, 0.0f, 0.4f);
    square1.GetComponent<ScarlettGame::SquareSprite>()->color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

    auto square2 = ScarlettGame::GameCore::Instance().CreateEntity();
    square2.GetComponent<ScarlettGame::Transform>()->translation = glm::vec3(-0.5f, 0.0f, 0.2f);
    square2.GetComponent<ScarlettGame::SquareSprite>()->color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

    auto square3 = ScarlettGame::GameCore::Instance().CreateEntity();
    square3.GetComponent<ScarlettGame::Transform>()->translation = glm::vec3(0.0f, -0.5f, 0.3f);
    square3.GetComponent<ScarlettGame::SquareSprite>()->color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);

    auto square4 = ScarlettGame::GameCore::Instance().CreateEntity();
    square4.GetComponent<ScarlettGame::Transform>()->translation = glm::vec3(0.0f, 0.5f, 0.1f);
    square4.GetComponent<ScarlettGame::SquareSprite>()->color = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);

    auto square5 = ScarlettGame::GameCore::Instance().CreateEntity();
    const auto transform = square5.GetComponent<ScarlettGame::Transform>();
    transform->translation = glm::vec3(0.0f, 0.0f, 0.0f);
    transform->scale = glm::vec3(0.5f, 1.f, 1.0f);
    transform->rotation = glm::vec3(0.0f, 0.0f, 45.0f);
    square5.GetComponent<ScarlettGame::SquareSprite>()->color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

    // Everything initialized okay, so we can run the engine.
    mRunning = true;
}

void Engine::Run() const
{
    while(mRunning)
    {
        mMainWindow->Update();

        Renderer::Instance().BeginRender();
        Renderer::Instance().Render();
        // Todo restructure renderer BeginRender and EndRender so systems can update pre-render.
        // Probably will happen when changing to command based rendering.
        ScarlettGame::GameCore::Instance().GetActiveScene()->Update();
        Renderer::Instance().EndRender();
    }
}

void Engine::DestroyEngine()
{
    ScarlettGame::GameCore::Destroy();

    Renderer::Instance().Destroy();

    WindowManager::DestroyWindow(mMainWindow);
    WindowManager::TerminateApi();

    mLayerStack->PopLayer(mWindowLayer);
    delete mLayerStack;
    mLayerStack = nullptr;

    SCARLETT_DLOG("Engine Destroyed.");
}

void Engine::OnEvent(Event& e) const
{
    mLayerStack->OnEvent(e);
}

} // Namespace Scarlett.
