#include "ScarlettEditorPch.h"
#include "EditorManager.h"

#include "Src/ScarlettEditorDefines.h"

namespace ScarlettEditor
{

EditorManager* EditorManager::mInstance = nullptr;

static ImVec4 HexToRgba(int a, int b, int c)
{
    return { a / 255.0f, b / 255.0f, c / 255.0f, 1.0f };
}

static ImVec4 Dark      = HexToRgba(0x17, 0x17, 0x17); // #171717FF
static ImVec4 Medium    = HexToRgba(0x2A, 0x2A, 0x2A); // #2A2A2AFF
static ImVec4 Light     = HexToRgba(0xA9, 0xA9, 0xA9);
static ImVec4 OffWhite  = HexToRgba(0xF5, 0xF5, 0xF5);

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

    // style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
    // style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.00f, 0.40f, 0.41f, 1.00f);
    style.Colors[ImGuiCol_WindowBg] = Dark;
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
    // style.Colors[ImGuiCol_Tab] = ImVec4(0.00f, 0.40f, 0.41f, 0.40f);
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

void EditorManager::RenderUi(const ImTextureID viewportTexture)
{
    ImGui::DockSpaceOverViewport();

    bool show = true;
    ImGui::ShowDemoWindow(&show);

    ImGui::Begin("Properties");

    if (ImGui::CollapsingHeader("Background Color"))
    {

    }

    ImGui::End();

    ImGui::Begin("Console");

    ImGui::End();


    ImGui::Begin("Scene");

    ImGui::End();

    ImGui::Begin("Viewport");

    static int num = 0;
    num++;

    const ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    ImGui::Image(viewportTexture, viewportPanelSize);

    ImGui::End();

}

} // Namespace ScarlettEditor.
