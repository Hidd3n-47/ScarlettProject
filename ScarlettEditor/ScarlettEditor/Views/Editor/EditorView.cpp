#include "ScarlettEditorPch.h"
#include "EditorView.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>

#include <ScarlEntt/Scene.h>

#include <ScarlettGameCore/Src/GameCore.h>
#include <ScarlettGameCore/Components/Tag.h>

#include "Editor/EditorManager.h"

#include "PropertiesPanel.h"
#include "ScenePanel.h"

#include "ViewportCameraSystem.h"
#include "ScarlettGameCore/Components/Camera.h"

namespace ScarlettEditor
{

//Todo should title be definied in the constructor.
EditorView::EditorView()
    : mPropertiesPanel(new PropertiesPanel(this, {.title = "Properties" }))
    , mScenePanel(new ScenePanel(this, {.title = "Scene" }))
{
    ScarlettGame::GameCore::Instance().GetActiveScene()->RegisterSystem<ViewportCameraSystem>();
    ScarlettGame::GameCore::Instance().GetActiveScene()->RegisterComponent<ViewportCamera>();

    const ScarlEntt::EntityHandle viewportCamera = ScarlettGame::GameCore::Instance().CreateEntity();
    viewportCamera.RemoveComponent<ScarlettGame::Tag>();
    ViewportCamera* camera = viewportCamera.AddComponent<ViewportCamera>();

    //todo change aspect ratio to be the viewport.
    camera->projectionMatrix = glm::perspective(glm::radians(60.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);
}

EditorView::~EditorView()
{
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

    ImGui::Begin("Console");

    ImGui::End();
}

void EditorView::RenderViewport() const
{
    ImGui::Begin("Viewport");

    const ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    ImGui::Image(EditorManager::Instance().GetViewportTexture(), viewportPanelSize);

    ImGui::End();
}

} // ScarlettEditor.


