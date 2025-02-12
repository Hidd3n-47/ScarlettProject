#include "ScarlettEnginePch.h"
#include "Engine.h"

#include "Core/Window/WindowManager.h"
#include "Rendering/Vulkan/VulkanRendererEditor.h"

namespace Scarlett
{

void Engine::InitEngine()
{
    Log::Init();

    WindowManager::InitApi();
    mMainWindow = WindowManager::CreateWindowInternal();

    //Renderer::Init(mMainWindow);
    mVulkRenderer = new VulkanRendererEditor();
    mVulkRenderer->Init(mMainWindow);

    SCARLETT_DLOG("Engine Initialized");

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
    //Renderer::Destroy();
    mVulkRenderer->Destroy();
    delete mVulkRenderer;
    mVulkRenderer = nullptr;

    WindowManager::DestroyWindow(mMainWindow);
    WindowManager::TerminateApi();

    SCARLETT_DLOG("Engine Destoryed.");
}

} // Namespace Scarlett.
