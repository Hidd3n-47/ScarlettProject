#include "ScarlettEditorPch.h"
#include "PropertiesPanel.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

#include <ScarlEntt/EntityHandle.h>

#include <ScarlettGame/Components/Tag.h>
#include <ScarlettGame/Components/Transform.h>
#include <ScarlettGame/Components/SquareSprite.h>

namespace ScarlettEditor
{

void PropertiesPanel::Render()
{
    //auto* selectedEntity = EditorManager::Instance().GetSelectionManager().GetSelectedEntity();
    ScarlEntt::EntityHandle* selectedEntity = nullptr;
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
}

}
