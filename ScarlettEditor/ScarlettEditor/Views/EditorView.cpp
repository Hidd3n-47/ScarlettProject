#include "ScarlettEditorPch.h"
#include "EditorView.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>

#include <ScarlEntt/Scene.h>

#include <ScarlettGameCore/Components/Tag.h>
#include <ScarlettGameCore/Components/Transform.h>
#include <ScarlettGameCore/Components/SquareSprite.h>

#include "Editor/EditorManager.h"

#include "PropertiesPanel.h"
#include "ScenePanel.h"

namespace ScarlettEditor
{

//Todo should title be definied in the constructor.
EditorView::EditorView()
    : mPropertiesPanel(new PropertiesPanel(this, {.title = "Properties" }))
    , mScenePanel(new ScenePanel(this, {.title = "Scene" }))
{
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


