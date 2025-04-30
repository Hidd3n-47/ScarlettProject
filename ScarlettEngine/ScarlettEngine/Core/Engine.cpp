#include "ScarlettEnginePch.h"
#include "Engine.h"

#include <ScarlettEditor/Editor/EditorManager.h>

#include <ScarlettGameCore/Src/GameCore.h>

#include <Components/Line.h>
#include <Components/Camera.h>
#include <Components/SquareSprite.h>

#include "WindowLayer.h"
#include "Input/LayerStack.h"

#include "Core/Window/WindowManager.h"
#include "Rendering/Vulkan/VulkanRendererEditor.h"

#include "Input/InputManager.h"
#include "Events/ApplicationEvents.h"

#include "Systems/CameraSystem.h"
#include "Systems/LineRendererSystem.h"
#include "Systems/SquareSpriteSystem.h"

namespace Scarlett
{

Engine* Engine::mInstance = nullptr;

void Engine::InitEngine()
{
    SCARLETT_ASSERT(mInstance == nullptr && "Engine has already been initialized, can only initialize engine once.");
    mInstance = this;

    mLayerStack = new LayerStack();

    std::string windowConfiguration;
#if defined(SCARLETT_DEBUG)
    windowConfiguration = " - Dev";
    Log::Init();
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

    ScarlettGame::GameCore::Instance().GetActiveScene()->RegisterSystem<SquareSpriteSystem>();
    ScarlettGame::GameCore::Instance().GetActiveScene()->RegisterSystem<LineRendererSystem>();

    Renderer::Instance().Init(mMainWindow);

    ScarlettGame::GameCore::Instance().GetActiveScene()->RegisterSystem<CameraSystem>();

    SCARLETT_DLOG("Engine Initialized");

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

        // Trigger OnUpdate Event.
        OnUpdateEvent event;
        OnEvent(event);
    }
}

void Engine::DestroyEngine()
{
    Renderer::Instance().Destroy();

    ScarlettGame::GameCore::Destroy();

    WindowManager::DestroyWindow(mMainWindow);
    WindowManager::TerminateApi();

    mLayerStack->PopLayer(mWindowLayer);
    delete mLayerStack;
    mLayerStack = nullptr;

    SCARLETT_DLOG("Engine Destroyed.");
}

void Engine::OnEvent(Event& e) const
{
#ifdef SCARLETT_EDITOR_ENABLED
    ScarlettEditor::EditorManager::Instance().OnEvent(e);
#endif // SCARLETT_EDITOR_ENABLED.

    InputManager::OnEvent(e);

    mLayerStack->OnEvent(e);
}

} // Namespace Scarlett.
