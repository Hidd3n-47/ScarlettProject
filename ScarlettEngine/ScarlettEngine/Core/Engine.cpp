#include "ScarlettEnginePch.h"
#include "Engine.h"

#include <ScarlEntt/Scene.h>

#include "Core/Window/WindowManager.h"
#include "Rendering/Vulkan/VulkanRendererEditor.h"

#include <ScarlettGame/Components/Tag.h>
#include <ScarlettGame/Components/Transform.h>
#include <ScarlettGame/Components/SquareSprite.h>
#include "Systems/SquareSpriteSystem.h"

namespace Scarlett
{

Engine* Engine::mInstance = nullptr;

void Engine::InitEngine()
{
    SCARLETT_ASSERT(mInstance == nullptr && "Engine has already been initialized, can only initialize engine once.");
    mInstance = this;

    Log::Init();

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

    mMainWindow->SetEventCallback(SCARLETT_BIND_FUNCTION(Engine::OnEvent));

    Renderer::Instance().Init(mMainWindow);

    SCARLETT_DLOG("Engine Initialized");
    mScene = new ScarlEntt::Scene();

    SquareSpriteSystemProperties properties;
    properties.device = Renderer::Instance().GetDevice();
    mScene->RegisterSystem<SquareSpriteSystem>(&properties);
    mScene->RegisterComponent<Tag>();
    mScene->RegisterComponent<Transform>();
    mScene->RegisterComponent<SquareSprite>();

    auto square1 = mScene->CreateEntity();
    square1.AddComponent<Tag>("Square1", square1);
    square1.AddComponent<Transform>()->translation = glm::vec3(0.5f, 0.0f, 0.4f);
    square1.AddComponent<SquareSprite>()->color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

    auto square2 = mScene->CreateEntity();
    square2.AddComponent<Tag>("Square2", square2);
    square2.AddComponent<Transform>()->translation = glm::vec3(-0.5f, 0.0f, 0.2f);
    square2.AddComponent<SquareSprite>()->color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

    auto square3 = mScene->CreateEntity();
    square3.AddComponent<Tag>("Square3", square3);
    square3.AddComponent<Transform>()->translation = glm::vec3(0.0f, -0.5f, 0.3f);
    square3.AddComponent<SquareSprite>()->color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);

    auto square4 = mScene->CreateEntity();
    square4.AddComponent<Tag>("Square4", square4);
    square4.AddComponent<Transform>()->translation = glm::vec3(0.0f, 0.5f, 0.1f);
    square4.AddComponent<SquareSprite>()->color = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);

    auto square5 = mScene->CreateEntity();
    square5.AddComponent<Tag>("Square5", square5);
    auto transform = square5.AddComponent<Transform>();
    transform->translation = glm::vec3(0.0f, 0.0f, 0.0f);
    transform->scale = glm::vec3(0.5f, 1.f, 1.0f);
    transform->rotation = glm::vec3(0.0f, 0.0f, 45.0f);
    square5.AddComponent<SquareSprite>()->color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

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
        // Probably will happen when changing to command based renderering.
        mScene->Update();
        Renderer::Instance().EndRender();
    }
}

void Engine::DestroyEngine()
{
    delete mScene;
    mScene = nullptr;

    Renderer::Instance().Destroy();

    WindowManager::DestroyWindow(mMainWindow);
    WindowManager::TerminateApi();

    SCARLETT_DLOG("Engine Destroyed.");
}

void Engine::OnEvent(Event& e)
{
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowClosedEvent>(SCARLETT_BIND_FUNCTION(Engine::OnWindowClose));
}

bool Engine::OnWindowClose(const WindowClosedEvent& e)
{
    mRunning = false;
    return true;
}

} // Namespace Scarlett.
