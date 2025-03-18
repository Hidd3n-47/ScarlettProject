#include "ScarlettEditorPch.h"
#include "EditorManager.h"

#include <ScarlEntt/Scene.h>

#include "Src/ScarlettEditorDefines.h"

#include "Views/EditorView.h"

namespace ScarlettEditor
{

EditorManager* EditorManager::mInstance = nullptr;

static ImVec4 HexToRgba(int a, int b, int c)
{
    return { a / 255.0f, b / 255.0f, c / 255.0f, 1.0f };
}

static ImVec4 Dark      = HexToRgba(0x20, 0x20, 0x20); // #171717FF
static ImVec4 Medium    = HexToRgba(0x30, 0x30, 0x30); // #2A2A2AFF
static ImVec4 aaa       = HexToRgba(0x40, 0x40, 0x40); // #2A2A2AFF
static ImVec4 Light     = HexToRgba(0x50, 0x50, 0x50);
static ImVec4 OffWhite  = HexToRgba(0xF5, 0xF5, 0xF5);
static ImVec4 Scarlett  = HexToRgba(0xDC, 0x33, 0xC6); // DC33C6

void EditorManager::Init(ImGuiContext* uiContext, ImGuiMemAllocFunc* allocateFunction, ImGuiMemFreeFunc* freeFunction, void* data)
{
    EDITOR_ASSERT(mInstance == nullptr && "ScarlettEditor has already been initialized, only initialize once.");

    mInstance = new EditorManager();

    Scarlett::Log::Init(true, "ScarlettEditor");

    ImGui::SetCurrentContext(uiContext);
    ImGui::SetAllocatorFunctions(*allocateFunction, *freeFunction, data);

    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    style.Alpha             = 1.0f;
    style.WindowRounding    = 0.0f;
    style.FrameRounding     = 0.0f;
    style.FramePadding = ImVec2(7.0f, 4.0f);
    style.WindowPadding = ImVec2(10.0f, 5.0f);
    style.WindowMenuButtonPosition = ImGuiDir_None;
    style.TabBarOverlineSize = 0.0f;

    style.Colors[ImGuiCol_TitleBg] = Dark;
    style.Colors[ImGuiCol_TitleBgActive] = Dark;

    style.Colors[ImGuiCol_WindowBg] = Medium;
    style.Colors[ImGuiCol_Tab] = Medium;

    style.Colors[ImGuiCol_TabActive] = Medium;
    style.Colors[ImGuiCol_TabSelected] = Medium;
    style.Colors[ImGuiCol_TabHovered] = Medium;
    style.Colors[ImGuiCol_TabDimmed] = Medium;
    style.Colors[ImGuiCol_TabDimmedSelected] = Medium;

    style.Colors[ImGuiCol_Button] = Light;
    style.Colors[ImGuiCol_ButtonActive] = Light;
    style.Colors[ImGuiCol_ButtonHovered] = Scarlett;

    style.Colors[ImGuiCol_Header] = Light;
    style.Colors[ImGuiCol_HeaderActive] = Light;
    style.Colors[ImGuiCol_HeaderHovered] = Scarlett;

    // style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
    // style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.00f, 0.40f, 0.41f, 1.00f);
    // style.Colors[ImGuiCol_Border] = ImVec4(0.00f, 1.00f, 1.00f, 0.65f);
    // style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    // style.Colors[ImGuiCol_FrameBg] = ImVec4(0.44f, 0.80f, 0.80f, 0.18f);
    // style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.44f, 0.80f, 0.80f, 0.27f);
    // style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.44f, 0.81f, 0.86f, 0.66f);
    // style.Colors[ImGuiCol_TitleBg] = ImVec4(0.14f, 0.18f, 0.21f, 0.73f);
    // style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.54f);
    // style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 1.00f, 1.00f, 0.27f);
    // style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.20f);
    // style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.22f, 0.29f, 0.30f, 0.71f);
    // style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.00f, 1.00f, 1.00f, 0.44f);
    // style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.00f, 1.00f, 1.00f, 0.74f);
    // style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
    // style.Colors[ImGuiCol_CheckMark] = ImVec4(0.00f, 1.00f, 1.00f, 0.68f);
    // style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.00f, 1.00f, 1.00f, 0.36f);
    // style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.00f, 1.00f, 1.00f, 0.76f);
    // style.Colors[ImGuiCol_Button] = ImVec4(0.00f, 0.65f, 0.65f, 0.46f);
    // style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.01f, 1.00f, 1.00f, 0.43f);
    // style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.00f, 1.00f, 1.00f, 0.62f);
    // style.Colors[ImGuiCol_Header] = ImVec4(0.00f, 1.00f, 1.00f, 0.33f);
    // style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 1.00f, 1.00f, 0.42f);
    // style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.00f, 1.00f, 1.00f, 0.54f);
    // style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 1.00f, 1.00f, 0.54f);
    // style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.00f, 1.00f, 1.00f, 0.74f);
    // style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
    // style.Colors[ImGuiCol_PlotLines] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
    // style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
    // style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
    // style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
    // style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 1.00f, 1.00f, 0.22f);
    // style.Colors[ImGuiCol_TabActive] = ImVec4(0.00f, 0.60f, 0.61f, 0.80f);
    // style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.00f, 0.40f, 0.41f, 1.00f);

    //Scarlett::Log::Init(true, "ScarlettEditor");

    EDITOR_ILOG("Editor Initialized.");
}

void EditorManager::Destroy()
{
    delete mInstance;
    mInstance = nullptr;

    EDITOR_ILOG("Editor Destroyed.");
}

void EditorManager::RenderUi(ScarlEntt::Scene* currentScene, const ImTextureID viewportTexture)
{
    mCurrentScene = currentScene;
    mViewportTexture = viewportTexture;

    if (mCurrentView == nullptr)
    {
        mCurrentView = new EditorView();
    }

    ((EditorView*)mCurrentView)->SetCurrentScene(currentScene);

    mCurrentView->Render();
}

} // Namespace ScarlettEditor.
