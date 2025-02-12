#include "Application.h"

#include <string>
#include <cstdio>
#include <iostream>

#include "imgui_internal.h"
#include <imgui_impl_sdlrenderer2.h>

#include <sdl/SDL.h>

#include "Color.h"
#include "TaskRegistry.h"

void Application::Init()
{
    TaskRegistry::Instance()->Init();

    SDL_Init(SDL_INIT_EVERYTHING);

    mWindow = SDL_CreateWindow("Scarlett Dev Ops", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1080, 720, SDL_WINDOW_ALLOW_HIGHDPI);
    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    mMainFont = io.Fonts->AddFontFromFileTTF("E:/Programming/Scarlett/ScarlettDevOps/Assets/Fonts/Oswald/static/Oswald-Regular.ttf", 25);
    mTitleFont = io.Fonts->AddFontFromFileTTF("E:/Programming/Scarlett/ScarlettDevOps/Assets/Fonts/Oswald/static/Oswald-Bold.ttf", 40);
    mPlusFont = io.Fonts->AddFontFromFileTTF("E:/Programming/Scarlett/ScarlettDevOps/Assets/Fonts/Oswald/static/Oswald-Bold.ttf", 32);

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForSDLRenderer(mWindow, mRenderer);
    ImGui_ImplSDLRenderer2_Init(mRenderer);
}

void Application::Run()
{
    while(mRunning)
    {
        Input();

        Render();
    }
}

void Application::Destroy()
{
    TaskRegistry::Instance()->Destroy();
}

void Application::Input()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT)
        {
            mRunning = false;
        }

        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(mWindow))
        {
            mRunning = true;
        }
    }
}

void Application::Render()
{
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    RenderUi();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::Render();
    SDL_RenderSetScale(mRenderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
    SDL_RenderClear(mRenderer);
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), mRenderer);
    SDL_RenderPresent(mRenderer);
}

void Application::RenderUi()
{
    ImGui::DockSpaceOverViewport();

    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x, main_viewport->WorkPos.y), 0, ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize(ImVec2(1080, 720));

    // > Window Start - Main Task Viewport.
    ImGuiWindowFlags mainWindowFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize;
    bool open = true;
    ImGui::Begin("Main Task Viewport", &open, mainWindowFlags);

    // >> Columns Start - Title.
    ImGui::BeginColumns("Title", 2, ImGuiOldColumnFlags_NoResize | ImGuiOldColumnFlags_NoBorder);
    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, { 3.0f, 3.0f });

    // >- Columns 1.
    const float TITLE_COLUMN_WIDTH = 1025;
    ImGui::SetColumnWidth(0, TITLE_COLUMN_WIDTH);

    ImGui::PushFont(mTitleFont);
    ImGui::PushStyleColor(ImGuiCol_Text, Colors::textSecondary.ImColor());
    const char* title = "Scarlett Dev Ops ";
    ImGui::SetCursorPosX(TITLE_COLUMN_WIDTH * 0.5f - static_cast<float>(strlen(title)) - 20);
    ImGui::SetCursorPosY(8.0f);
    ImGui::Text(title);
    ImGui::PopStyleColor();
    ImGui::PopFont();

    // >- Columns 2.
    ImGui::NextColumn();

    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 7.0f);

    ImGui::PushStyleColor(ImGuiCol_Button, Colors::button.ImColor());
    Color hoverColor = mState == UiState::TaskView ? Colors::buttonHover : Colors::button;
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, hoverColor.ImColor());

    ImGui::PushFont(mPlusFont);
    ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, { 0.5f, 0.5f });
    if(ImGui::ButtonEx("+", { 35.0f,35.0f }, ImGuiButtonFlags_PressedOnRelease))
    {
        mState = UiState::AddingTask;
    }
    ImGui::PopStyleVar();
    ImGui::PopFont();

    ImGui::PopStyleColor();
    ImGui::PopStyleColor();

    if(mState == UiState::AddingTask)
    {
        if(TaskRegistry::Instance()->AddTaskUi())
        {
            mState = UiState::TaskView;
        }
    }

    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    ImGui::EndColumns();
    // >> Columns End - Title.
    TaskRegistry::Instance()->RenderTasks();

    ImGui::End();
}
