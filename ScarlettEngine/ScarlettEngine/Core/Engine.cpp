#include "ScarlettEnginePch.h"
#include "Engine.h"

#include "Core/Window/WindowManager.h"
#include "Rendering/Vulkan/Renderer.h"

namespace Scarlett
{

void Engine::InitEngine()
{
    Log::Init();

    WindowManager::InitApi();
    mMainWindow = WindowManager::CreateWindowInternal();

    Renderer::Init(mMainWindow);

    SCARLETT_DLOG("Engine Initialized");

    // Everything initialized okay, so we can run the engine.
    mRunning = true;
}

void Engine::Run() const
{
    while(mRunning)
    {
        mMainWindow->Update();
    }
}

void Engine::DestroyEngine() const
{
    Renderer::Destroy();

    WindowManager::DestroyWindow(mMainWindow);
    WindowManager::TerminateApi();

    SCARLETT_DLOG("Engine Destoryed.");
}

} // Namespace Scarlett.
