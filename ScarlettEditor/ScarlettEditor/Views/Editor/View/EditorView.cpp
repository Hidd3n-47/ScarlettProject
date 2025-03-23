#include "ScarlettEditorPch.h"
#include "EditorView.h"

#include <imgui/imgui.h>

#include <ScarlEntt/Scene.h>

#include <ScarlettGameCore/Src/GameCore.h>
#include <ScarlettGameCore/Components/Tag.h>

#include "Editor/EditorManager.h"

#include "Views/Editor/Panels/PropertiesPanel.h"
#include "Views/Editor/Panels/ScenePanel.h"

#include "Views/Editor/ViewportCameraSystem.h"
#include "ScarlettGameCore/Components/Camera.h"

#include "Views/Editor/Input/EditorViewInputLayer.h"

namespace ScarlettEditor
{

//Todo should title be defined in the constructor.
EditorView::EditorView()
    : mPropertiesPanel(new PropertiesPanel(this, {.title = "Properties" }))
    , mScenePanel(new ScenePanel(this, {.title = "Scene" }))
{
    ScarlettGame::GameCore::Instance().GetActiveScene()->RegisterSystem<ViewportCameraSystem>();
    ScarlettGame::GameCore::Instance().GetActiveScene()->RegisterComponent<ViewportCamera>();

    const ScarlEntt::EntityHandle viewportCamera = ScarlettGame::GameCore::Instance().CreateEntity();
    viewportCamera.RemoveComponent<ScarlettGame::Tag>();
    (void)viewportCamera.AddComponent<ViewportCamera>();

    mEditorViewOverlay = EditorManager::Instance().GetLayerStack()->PushOverlay<EditorViewInputLayer>(this);
}

EditorView::~EditorView()
{
    EditorManager::Instance().GetLayerStack()->PopOverlay(mEditorViewOverlay);

    delete mPropertiesPanel;
    delete mScenePanel;
}

void EditorView::Render()
{
    ImGui::DockSpaceOverViewport();

    bool show = true;
    ImGui::ShowDemoWindow(&show);

    RenderViewport();
    mPropertiesPanel->RenderUi();
    mScenePanel->RenderUi();

    // todo move the console into a panel.
    ImGui::Begin("Console");

    ImGui::End();
}

// todo move this into a panel.
void EditorView::RenderViewport()
{
    ImGui::Begin("Viewport");

    //todo this doesn't return true? why?
    mInFocus = ImGui::IsWindowFocused();
    mIsHovered = ImGui::IsWindowHovered();

    const ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    ImGui::Image(EditorManager::Instance().GetViewportTexture(), viewportPanelSize);

    ImGui::End();
}

} // ScarlettEditor.


