#include "ScarlettEditorPch.h"
#include "PropertiesPanel.h"

#include <imgui/imgui.h>

#include <ScarlEntt/EntityHandle.h>

#include <ScarlettGameCore/Components/Tag.h>
#include <ScarlettGameCore/Components/Transform.h>
#include <ScarlettGameCore/Components/SquareSprite.h>

#include "Views/Editor/View/EditorView.h"

namespace ScarlettEditor
{

void PropertiesPanel::Render()
{
    auto* selectedEntity = dynamic_cast<EditorView*>(mView)->GetSelectionManager().GetSelectedEntity();

    if (selectedEntity != nullptr)
    {
        const std::string entityName = selectedEntity->GetComponent<ScarlettGame::Tag>()->name.c_str();

        if (ImGui::CollapsingHeader("Transform"))
        {
            auto* transform = selectedEntity->GetComponent<ScarlettGame::Transform>();
            ImGui::DragFloat3((std::string("Position##") + entityName).c_str(), &transform->translation.x, 0.05f);
            ImGui::DragFloat3((std::string("Rotation##") + entityName).c_str(), &transform->rotation.x, 0.05f);
            ImGui::DragFloat3((std::string("Scale##") + entityName).c_str(), &transform->scale.x, 0.05f);
        }

        if (ImGui::CollapsingHeader("Square Sprite"))
        {
            auto* squareSprite = selectedEntity->GetComponent<ScarlettGame::SquareSprite>();
            ImGui::DragFloat3((std::string("Colour##") + entityName).c_str(), &squareSprite->color.x, 0.01f);
        }
    }
}

}
