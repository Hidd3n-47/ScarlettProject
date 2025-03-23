#include "ScarlettEditorPch.h"
#include "EditorManager.h"

#include <ScarlEntt/Scene.h>

#include <Core/Input/LayerStack.h>

#include "Src/ScarlettEditorDefines.h"

#include "Views/Editor/View/EditorView.h"

namespace ScarlettEditor
{

EditorManager* EditorManager::mInstance     = nullptr;

static ImVec4 HexToRgba(const int a, const int b, const int c)
{
    return { static_cast<float>(a) / 255.0f, static_cast<float>(b) / 255.0f, static_cast<float>(c) / 255.0f, 1.0f };
}

static const ImVec4 DARK      = HexToRgba(0x20, 0x20, 0x20); // #171717FF
static const ImVec4 MEDIUM    = HexToRgba(0x30, 0x30, 0x30); // #2A2A2AFF
//static const ImVec4 AAA       = HexToRgba(0x40, 0x40, 0x40); // #2A2A2AFF
static const ImVec4 LIGHT     = HexToRgba(0x50, 0x50, 0x50);
//static const ImVec4 OFF_WHITE = HexToRgba(0xF5, 0xF5, 0xF5);
static const ImVec4 SCARLETT  = HexToRgba(0xDC, 0x33, 0xC6); // DC33C6

EditorManager::~EditorManager()
{
    EDITOR_ILOG("Editor Destroyed.");

    delete mCurrentView;
    delete mEditorLayerStack;
}

// ImGui Alloc and free cannot be marked as const else ImGui fails to init. Disabling the warning.
// ReSharper disable twice CppParameterMayBeConstPtrOrRef
void EditorManager::CreateInstance(ImGuiContext* uiContext, ImGuiMemAllocFunc* allocateFunction, ImGuiMemFreeFunc* freeFunction, void* data)
{
    EDITOR_ASSERT(mInstance == nullptr && "ScarlettEditor has already been initialized, only initialize once.");

    mInstance = new EditorManager();

    Scarlett::Log::Init(true, "ScarlettEditor");

    ImGui::SetCurrentContext(uiContext);
    ImGui::SetAllocatorFunctions(*allocateFunction, *freeFunction, data);

    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    style.Alpha                     = 1.0f;
    style.WindowRounding            = 0.0f;
    style.FrameRounding             = 0.0f;
    style.FramePadding              = ImVec2(7.0f, 4.0f);
    style.WindowPadding             = ImVec2(10.0f, 5.0f);
    style.WindowMenuButtonPosition  = ImGuiDir_None;
    style.TabBarOverlineSize        = 0.0f;

    style.Colors[ImGuiCol_TitleBg]              = DARK;
    style.Colors[ImGuiCol_TitleBgActive]        = DARK;

    style.Colors[ImGuiCol_WindowBg]             = MEDIUM;
    style.Colors[ImGuiCol_Tab]                  = MEDIUM;

    style.Colors[ImGuiCol_TabActive]            = MEDIUM;
    style.Colors[ImGuiCol_TabSelected]          = MEDIUM;
    style.Colors[ImGuiCol_TabHovered]           = MEDIUM;
    style.Colors[ImGuiCol_TabDimmed]            = MEDIUM;
    style.Colors[ImGuiCol_TabDimmedSelected]    = MEDIUM;

    style.Colors[ImGuiCol_Button]               = LIGHT;
    style.Colors[ImGuiCol_ButtonActive]         = LIGHT;
    style.Colors[ImGuiCol_ButtonHovered]        = SCARLETT;

    style.Colors[ImGuiCol_Header]               = LIGHT;
    style.Colors[ImGuiCol_HeaderActive]         = LIGHT;
    style.Colors[ImGuiCol_HeaderHovered]        = SCARLETT;

    // style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
    // style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.00f, 0.40f, 0.41f, 1.00f);
    // style.Colors[ImGuiCol_Border] = ImVec4(0.00f, 1.00f, 1.00f, 0.65f);
    // style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    // style.Colors[ImGuiCol_FrameBg] = ImVec4(0.44f, 0.80f, 0.80f, 0.18f);
    // style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.44f, 0.80f, 0.80f, 0.27f);
    // style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.44f, 0.81f, 0.86f, 0.66f);
    // style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.54f);
    // style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.20f);
    // style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.22f, 0.29f, 0.30f, 0.71f);
    // style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.00f, 1.00f, 1.00f, 0.44f);
    // style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.00f, 1.00f, 1.00f, 0.74f);
    // style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
    // style.Colors[ImGuiCol_CheckMark] = ImVec4(0.00f, 1.00f, 1.00f, 0.68f);
    // style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.00f, 1.00f, 1.00f, 0.36f);
    // style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.00f, 1.00f, 1.00f, 0.76f);
    // style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 1.00f, 1.00f, 0.54f);
    // style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.00f, 1.00f, 1.00f, 0.74f);
    // style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
    // style.Colors[ImGuiCol_PlotLines] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
    // style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
    // style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
    // style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
    // style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 1.00f, 1.00f, 0.22f);
    // style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.00f, 0.40f, 0.41f, 1.00f);

    EDITOR_ILOG("Editor Instance Created.");
}

void EditorManager::DestroyInstance()
{
    delete mInstance;
    mInstance = nullptr;

    EDITOR_ILOG("Editor Instance Destroyed.");
}

void EditorManager::Init()
{
    mEditorLayerStack = new Scarlett::LayerStack();
    mCurrentView = new EditorView();

    EDITOR_ILOG("Editor Initialized.");
}

void EditorManager::OnEvent(Scarlett::Event& e) const
{
    mEditorLayerStack->OnEvent(e);
}

void EditorManager::RenderUi(const ImTextureID viewportTexture)
{
    mViewportTexture = viewportTexture;

    mCurrentView->Render();
}

} // Namespace ScarlettEditor.
