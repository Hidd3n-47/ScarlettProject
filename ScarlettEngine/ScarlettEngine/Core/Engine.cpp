#include "ScarlettEnginePch.h"
#include "Engine.h"

#include <ScarlEntt/Scene.h>
#include <ScarlettLogger/Log.h>

#include "Core/Window/WindowManager.h"
#include "Rendering/Vulkan/VulkanRendererEditor.h"

#include "Components/Transform.h"

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

#ifdef SCARLETT_EDITOR_ENABLED
    mVulkRenderer = new VulkanRendererEditor();
#else
    mVulkRenderer = new VulkanRenderer();
#endif // SCARLETT_EDITOR_ENABLED.

    mVulkRenderer->Init(mMainWindow);

    SCARLETT_DLOG("Engine Initialized");
    mScene = new ScarlEntt::Scene();

    mScene->RegisterComponent<Transform>();

    auto square1 = mScene->CreateEntity();
    square1.AddComponent<Transform>();
    auto square2 = mScene->CreateEntity();
    square2.AddComponent<Transform>();

    // Everything initialized okay, so we can run the engine.
    mRunning = true;
}

void Engine::Run() const
{
    while(mRunning)
    {
        mMainWindow->Update();

        mVulkRenderer->BeginRender();
        mVulkRenderer->Render();
        mVulkRenderer->EndRender();
    }
}

void Engine::DestroyEngine()
{
    delete mScene;
    mScene = nullptr;

    mVulkRenderer->Destroy();
    delete mVulkRenderer;
    mVulkRenderer = nullptr;

    WindowManager::DestroyWindow(mMainWindow);
    WindowManager::TerminateApi();

    SCARLETT_DLOG("Engine Destoryed.");
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
