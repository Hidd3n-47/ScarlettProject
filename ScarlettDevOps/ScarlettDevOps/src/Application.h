#pragma once

#include <imgui_impl_sdl2.h>

enum class UiState
{
    TaskView,
    AddingTask,
    ViewingTask
};

class Application
{
public:
    Application() = default;
    ~Application() = default;

    void Init();
    void Run();
    void Destroy();

private:
    bool mRunning = true;

    void Input();
    void Render();
    void RenderUi();

    UiState mState = UiState::TaskView;

    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;

    ImFont* mMainFont;
    ImFont* mTitleFont;
    ImFont* mPlusFont;
};

