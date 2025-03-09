#include "ScarlettEditorPch.h"
#include "EditorView.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>

#include <ScarlEntt/Scene.h>

#include <ScarlettGame/Components/Tag.h>
#include <ScarlettGame/Components/Transform.h>
#include <ScarlettGame/Components/SquareSprite.h>

#include "Editor/EditorManager.h"

namespace ScarlettEditor
{

void EditorView::Render()
{
    ImGui::DockSpaceOverViewport();

    bool show = true;
    ImGui::ShowDemoWindow(&show);

    RenderViewport();
    RenderProperties();
    RenderSceneGraph();

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

void EditorView::RenderProperties() const
{
    ImGui::Begin("Properties");

    auto* selectedEntity = mSelectionManager.GetSelectedEntity();
    if (selectedEntity != nullptr)
    {
        const std::string entityName = selectedEntity->GetComponent<Scarlett::Tag>()->name;

        if (ImGui::CollapsingHeader("Transform"))
        {
            auto* transform = selectedEntity->GetComponent<Scarlett::Transform>();
            ImGui::DragFloat3((std::string("Position##") + entityName).c_str(), glm::value_ptr(transform->translation), 0.05f);
            ImGui::DragFloat3((std::string("Rotation##") + entityName).c_str(), glm::value_ptr(transform->rotation), 0.05f);
            ImGui::DragFloat3((std::string("Scale##") + entityName).c_str(), glm::value_ptr(transform->scale), 0.05f);
        }

        if (ImGui::CollapsingHeader("Square Sprite"))
        {
            auto* squareSprite = selectedEntity->GetComponent<Scarlett::SquareSprite>();
            ImGui::DragFloat3((std::string("Colour##") + entityName).c_str(), glm::value_ptr(squareSprite->color), 0.01f);
        }
    }


    ImGui::End();
}

void EditorView::RenderSceneGraph()
{
    ScarlEntt::ComponentManager* componentManager = mCurrentScene->GetComponentManager();
    auto& tags = componentManager->GetComponentArray<Scarlett::Tag>();

    ImGui::Begin("Scene");

    static int addedEntities = 0;
    if (ImGui::Button("Add Entity"))
    {
        const auto entity = EditorManager::Instance().GetCurrentScene()->CreateEntity();
        entity.AddComponent<Scarlett::Transform>();
        entity.AddComponent<Scarlett::Tag>("Entity", entity);
        entity.AddComponent<Scarlett::SquareSprite>();
    }

    bool selectedEntity = false;
    for (ScarlEntt::ComponentId i {0}; i < tags.Size(); ++i)
    {
         if (ImGui::CollapsingHeader(tags[i].name))
         {
             selectedEntity = true;
             mSelectionManager.SetSelectedEntity(&tags[i].entity);
         }
    }

    if (!selectedEntity)
    {
        mSelectionManager.SetSelectedEntity(nullptr);
    }

    ImGui::End();
}

} // ScarlettEditor.


