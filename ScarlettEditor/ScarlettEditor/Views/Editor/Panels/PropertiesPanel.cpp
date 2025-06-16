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

        //for (ScarlEntt::ComponentView& component : selectedEntity->GetComponents())
        //{
        //    if (ImGui::CollapsingHeader((component.GetComponentTypeId()).c_str()))
        //    {
        //        const auto properties = component.GetProperties();
        //        for (const auto& [propertyName, property] : properties)
        //        {
        //            //const std::string valueString = { property.GetValue(component.GetRawComponent()) };
        //            {
        //                const auto t = property.GetType();
        //            }
        //            switch (property.GetType())
        //            {
        //            case ScarlEntt::ValueType::FLOAT:
        //            {
        //                //auto value = ScarlEntt::TypeReflection::GetValueFromTypeString<float>(valueString);
        //                float value;
        //                ImGui::DragFloat((propertyName + "##" + entityName).c_str(), &value, 0.05f);
        //                break;
        //            }
        //            case ScarlEntt::ValueType::STRING:
        //            {
        //                //todo implement
        //                //auto value = ScarlEntt::TypeReflection::GetValueFromTypeString<std::string>(valueString);
        //                break;
        //            }
        //            case ScarlEntt::ValueType::VEC3:
        //            {
        //                //auto value = ScarlEntt::TypeReflection::GetValueFromTypeString<ScarlettMath::Vec3>(valueString);
        //                ScarlettMath::Vec3 value;
        //                ImGui::DragFloat3((propertyName + "##" + entityName).c_str(), &value.x, 0.05f);
        //                break;
        //            }
        //            case ScarlEntt::ValueType::VEC4:
        //            {
        //                ScarlettMath::Vec4 value;
        //                ScarlEntt::TypeReflection::GetValueFromProperty(property, component, value);
        //                //auto value = ScarlEntt::TypeReflection::GetValueFromTypeString<ScarlettMath::Vec4>(valueString);
        //                ImGui::DragFloat4((propertyName + "##" + entityName).c_str(), &value.x, 0.05f);
        //                break;
        //            }
        //            case ScarlEntt::ValueType::QUAT:
        //            {
        //                ScarlettMath::Vec4 value;
        //                //auto value = ScarlEntt::TypeReflection::GetValueFromTypeString<ScarlettMath::Quat>(valueString).ToVector4();
        //                ImGui::DragFloat4((propertyName + "##" + entityName).c_str(), &value.x, 0.05f);
        //                break;
        //            }
        //            case ScarlEntt::ValueType::UNKNOWN:
        //            default:
        //                // todo add logging or debug assert/breakpoint here.
        //                break;
        //            }
        //        }
        //    }
        //}
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
