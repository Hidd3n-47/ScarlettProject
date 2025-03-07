#include "ScarlettEnginePch.h"
#include "Engine.h"

#include <ScarlEntt/Scene.h>
#include <ScarlettLogger/Log.h>

#include "Core/Window/WindowManager.h"
#include "Rendering/Vulkan/VulkanRendererEditor.h"

#include "Components/Transform.h"
#include "Components/SquareSprite.h"
#include "Systems/SquareSpriteSystem.h"

namespace Scarlett
{

void Engine::InitEngine()
{
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
    mScene->RegisterComponent<Transform>();
    mScene->RegisterComponent<SquareSprite>();
    auto square1 = mScene->CreateEntity();
    square1.AddComponent<Transform>()->translation = glm::vec3(0.5f, 0.0f, 0.0f);
    square1.AddComponent<SquareSprite>()->color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    auto square2 = mScene->CreateEntity();
    square2.AddComponent<Transform>()->translation = glm::vec3(-0.5f, 0.0f, 0.0f);
    square2.AddComponent<SquareSprite>()->color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

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
        // Todo restructure renderer BeginRender and EndRender so systems can update pre-render. \
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
