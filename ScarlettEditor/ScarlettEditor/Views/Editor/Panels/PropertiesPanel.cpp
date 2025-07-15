#include "ScarlettEditorPch.h"
#include "PropertiesPanel.h"

#include <imgui/imgui.h>

#include <ScarlettGameCore/Src/GameCore.h>

#include <RTTI/TypeReflection.h>
#include <ScarlEntt/EntityHandle.h>

#include <Components/Tag.h>

#include "UI/UiControls.h"
#include "Views/Editor/View/EditorView.h"


namespace ScarlettEditor
{

void PropertiesPanel::Render()
{
    const ScarlEntt::EntityHandle* selectedEntity = dynamic_cast<EditorView*>(mView)->GetSelectionManager().GetSelectedEntity();

    if (selectedEntity)
    {
        const std::string entityName = selectedEntity->GetComponent<Scarlett::Component::Tag>()->name;

        for (ScarlEntt::ComponentView& component : *selectedEntity->GetComponents())
        {
            if (ImGui::CollapsingHeader(component.GetComponentTypeId()->FriendlyName().c_str()))
            {
                for (const auto& [propertyName, property] : *component.GetProperties())
                {
                    switch (property.GetType())
                    {
                    case ScarlEntt::PropertyType::FLOAT:
                    {
                        // todo implement with new way.
                        float value;
                        ScarlEntt::TypeReflection::SetValueFromString(value, property.GetPropertyValue());
                        break;
                    }
                    case ScarlEntt::PropertyType::STRING:
                        //todo implement
                        //auto value = ScarlEntt::TypeReflection::GetValueFromTypeString<std::string>(valueString);
                        break;
                    case ScarlEntt::PropertyType::VEC3:
                        UiControls::RenderVec3PropertyControl(property, {.propertyName = propertyName, .propertyId = entityName });
                        break;
                    case ScarlEntt::PropertyType::VEC4:
                        UiControls::RenderVec4PropertyControl(property, { .propertyName = propertyName, .propertyId = entityName });
                        break;
                    case ScarlEntt::PropertyType::QUAT:
                    {
                        // Todo Change this to render as a vec 3.
                        UiControls::RenderVec4PropertyControl(property, { .propertyName = propertyName, .propertyId = entityName });
                        //ScarlettMath::Vec4 value;
                        //ScarlEntt::TypeReflection::SetValueFromString(value, property.GetPropertyValue());
                        //ImGui::DragFloat4((propertyName + "##" + entityName).c_str(), &value.x, 0.05f);
                        break;
                    }
                    default:
                        // todo add logging or debug assert/breakpoint here.
                        break;
                    }
                }
            }
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
            /*for(const ScarlEntt::ComponentTypeId& component : ScarlettGame::GameCore::Instance().GetActiveScene()->GetComponentManager()->GetRegisteredComponentTypes())
            {
                if (ImGui::MenuItem(component.FriendlyName().c_str()))
                {
                    selectedEntity->AddDefaultComponent(component);
                    ImGui::CloseCurrentPopup();
                }
            }*/
            ImGui::EndMenu();
        }
        ImGui::EndPopup();
    }
}

} // Namespace ScarlettEditor.
