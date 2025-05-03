#include "ScarlettEditorPch.h"
#include "PropertiesPanel.h"

#include <imgui/imgui.h>

#include <ScarlettGameCore/Src/GameCore.h>

#include <ScarlEntt/EntityHandle.h>

#include <Components/Tag.h>
#include <Components/Transform.h>
#include <Components/SquareSprite.h>

#include "Views/Editor/View/EditorView.h"


namespace ScarlettEditor
{

void PropertiesPanel::Render()
{
    const ScarlEntt::EntityHandle* selectedEntity = dynamic_cast<EditorView*>(mView)->GetSelectionManager().GetSelectedEntity();

    if (selectedEntity)
    {
        const std::string entityName = selectedEntity->GetComponent<Scarlett::Component::Tag>()->name;

        if (ImGui::CollapsingHeader("Transform"))
        {
            const auto transform = selectedEntity->GetComponent<Scarlett::Component::Transform>();

            ImGui::DragFloat3((std::string("Position##") + entityName).c_str(), &transform->translation.x, 0.05f);

            float rot[3] = { 0.0f, 0.0f, 0.0f };
            transform->rotation.GetYawPitchRoll(rot[1], rot[0], rot[2]);
            rot[0] = ScarlettMath::Degrees(rot[0]);
            rot[1] = ScarlettMath::Degrees(rot[1]);
            rot[2] = ScarlettMath::Degrees(rot[2]);
            ImGui::DragFloat3((std::string("Rotation##") + entityName).c_str(), rot, 0.5f);
            transform->rotation.SetYawPitchRoll(ScarlettMath::Radians(rot[1]), ScarlettMath::Radians(rot[0]), ScarlettMath::Radians(rot[2]));

            ImGui::DragFloat3((std::string("Scale##") + entityName).c_str(), &transform->scale.x, 0.05f);
        }

        if (ImGui::CollapsingHeader("Square Sprite"))
        {
            const auto squareSprite = selectedEntity->GetComponent<Scarlett::Component::SquareSprite>();
            ImGui::DragFloat3((std::string("Colour##") + entityName).c_str(), &squareSprite->color.x, 0.01f);
        }
    }
}

void PropertiesPanel::RenderContextMenu()
{
    if (ImGui::BeginPopupContextWindow())
    {
        const ScarlEntt::EntityHandle* selectedEntity = dynamic_cast<EditorView*>(mView)->GetSelectionManager().GetSelectedEntity();

        if (!selectedEntity)
        {
            ImGui::CloseCurrentPopup();
            return;
        }

        if (ImGui::BeginMenu("Add Component"))
        {
            for(const ScarlEntt::ComponentTypeId& component : ScarlettGame::GameCore::Instance().GetActiveScene()->GetComponentManager()->GetRegisteredComponentTypes())
            {
                if (ImGui::MenuItem(component.FriendlyName().c_str()))
                {
                    selectedEntity->AddDefaultComponent(component);
                    ImGui::CloseCurrentPopup();
                }
            }
            ImGui::EndMenu();
        }
        ImGui::EndPopup();
    }
}

} // Namespace ScarlettEditor.
