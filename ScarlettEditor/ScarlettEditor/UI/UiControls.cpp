#include "ScarlettEditorPch.h"
#include "UiControls.h"

#include <imgui/imgui_internal.h>

#include <Math/Math.h>

#include <RTTI/Property.h>
#include <RTTI/TypeReflection.h>

#include "UiControlProperties.h"
#include "UiButtonControlProperties.h"
#include "Core/MaterialManager/EditorMaterialManager.h"
#include "Editor/EditorManager.h"

namespace ScarlettEditor
{

void UiControls::RenderVec3PropertyControl(const ScarlEntt::Property& property, const UiControlProperties& controlProperties /* = {} */)
{
    ScarlettMath::Vec3 value;
    ScarlEntt::TypeReflection::SetValueFromString(value, property.GetPropertyValue());

    std::string propertyName = controlProperties.propertyName;
    if (controlProperties.firstLetterUppercase)
    {
        propertyName[0] = static_cast<char>(std::toupper(propertyName[0]));
    }

    const std::string propertyId = controlProperties.propertyName + controlProperties.propertyId;
    const float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

    ImGui::Columns(2, propertyName.c_str());

    // Todo: Let this width be draggable/adjustable.
    ImGui::SetColumnWidth(0, 100.0f);

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + lineHeight - ImGui::CalcTextSize(propertyName.c_str()).y - ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.y + 5.0f);
    ImGui::Text("%s", propertyName.c_str());

    ImGui::NextColumn();

    ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0.0f, 4.0f });
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4);
    UiButtonControlProperties buttonProperties{ .buttonSize = {lineHeight + 3.0f, lineHeight} };

    buttonProperties.buttonColor        = ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f };
    buttonProperties.buttonHoveredColor = ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f };
    RenderFloatPropertyControl("X", propertyId, value.x, controlProperties, buttonProperties);
    ImGui::SameLine();

    buttonProperties.buttonColor        = ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f };
    buttonProperties.buttonHoveredColor = ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f };
    RenderFloatPropertyControl("Y", propertyId, value.y, controlProperties, buttonProperties);
    ImGui::SameLine();

    buttonProperties.buttonColor        = ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f };
    buttonProperties.buttonHoveredColor = ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f };
    RenderFloatPropertyControl("Z", propertyId, value.z, controlProperties, buttonProperties);

    ImGui::PopStyleVar(2);
    ImGui::PopItemWidth();
    ImGui::Columns(1);

    property.SetPropertyValue(ScarlEntt::TypeReflection::GetStringFromValue(value));
}

// todo, this is a copy-paste of above, would be good to have this being generic with maybe vector for the number of floats -> this would be more generic and possibly work for 1-n number of float control.
void UiControls::RenderVec4PropertyControl(const ScarlEntt::Property& property, const UiControlProperties& controlProperties /* = {} */)
{
    ScarlettMath::Vec4 value;
    ScarlEntt::TypeReflection::SetValueFromString(value, property.GetPropertyValue());

    std::string propertyName = controlProperties.propertyName;
    if (controlProperties.firstLetterUppercase)
    {
        propertyName[0] = static_cast<char>(std::toupper(propertyName[0]));
    }

    const std::string propertyId = controlProperties.propertyName + controlProperties.propertyId;
    const float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

    ImGui::Columns(2, propertyName.c_str());

    // Todo: Let this width be draggable/adjustable.
    ImGui::SetColumnWidth(0, 100.0f);

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + lineHeight - ImGui::CalcTextSize(propertyName.c_str()).y - ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.y + 5.0f);
    ImGui::Text("%s", propertyName.c_str());

    ImGui::NextColumn();

    ImGui::PushMultiItemsWidths(4, ImGui::CalcItemWidth());
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0.0f, 4.0f });
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4);
    UiButtonControlProperties buttonProperties{ .buttonSize = {lineHeight + 3.0f, lineHeight} };

    buttonProperties.buttonColor = ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f };
    buttonProperties.buttonHoveredColor = ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f };
    RenderFloatPropertyControl("X", propertyId, value.x, controlProperties, buttonProperties);
    ImGui::SameLine();

    buttonProperties.buttonColor = ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f };
    buttonProperties.buttonHoveredColor = ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f };
    RenderFloatPropertyControl("Y", propertyId, value.y, controlProperties, buttonProperties);
    ImGui::SameLine();

    buttonProperties.buttonColor = ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f };
    buttonProperties.buttonHoveredColor = ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f };
    RenderFloatPropertyControl("Z", propertyId, value.z, controlProperties, buttonProperties);

    ImGui::SameLine();
    buttonProperties.buttonColor = ImVec4{ 0.6f, 0.6f, 0.65f, 1.0f };
    buttonProperties.buttonHoveredColor = ImVec4{ 0.7f, 0.7f, 0.75f, 1.0f };
    RenderFloatPropertyControl("W", propertyId, value.z, controlProperties, buttonProperties);

    ImGui::PopStyleVar(2);
    ImGui::PopItemWidth();
    ImGui::Columns(1);

    property.SetPropertyValue(ScarlEntt::TypeReflection::GetStringFromValue(value));
}

void UiControls::RenderMaterialPropertyControl(const ScarlEntt::Property& property, const UiControlProperties& controlProperties /* = {} */)
{
    Scarlett::Material material;
    ScarlEntt::TypeReflection::SetValueFromString(material, property.GetPropertyValue());

    std::string propertyName = controlProperties.propertyName;
    if (controlProperties.firstLetterUppercase)
    {
        propertyName[0] = static_cast<char>(std::toupper(propertyName[0]));
    }

    const std::string propertyId = controlProperties.propertyName + controlProperties.propertyId;
    const float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

    ImGui::Columns(2, propertyName.c_str());

    // Todo: Let this width be draggable/adjustable.
    ImGui::SetColumnWidth(0, 100.0f);

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + lineHeight - ImGui::CalcTextSize(propertyName.c_str()).y - ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.y + 5.0f);
    ImGui::Text("%s", propertyName.c_str());

    ImGui::NextColumn();

    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3);

    const std::string currentMaterialName = EditorManager::Instance().GetMaterialManager()->GetMaterialCreateInfo(material.GetUlid()).friendlyName;
    if (ImGui::BeginCombo("##combo 1", currentMaterialName.c_str(), 0))
    {
        for (const auto& [materialUlid, materialInfo] : EditorManager::Instance().GetMaterialManager()->GetProcessedMaterialMap())
        {
            const bool selected = materialUlid == material.GetUlid();

            if (ImGui::Selectable(materialInfo.friendlyName.c_str(), selected) && materialInfo.ulid != material.GetUlid())
            {
                material.SetUlid(materialInfo.ulid);
            }

            if (selected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    ImGui::PopStyleVar();
    ImGui::Columns(1);

    property.SetPropertyValue(ScarlEntt::TypeReflection::GetStringFromValue(material));
}

void UiControls::RenderFloatPropertyControl(const std::string& label, const std::string& propertyId, float& value, const UiControlProperties& controlProperties, const UiButtonControlProperties& buttonProperties)
{
    ImGui::PushStyleColor(ImGuiCol_Button, buttonProperties.buttonColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, buttonProperties.buttonHoveredColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, buttonProperties.buttonColor);
    if (ImGui::Button((label +"##" + propertyId).c_str(), buttonProperties.buttonSize))
    {
        value = 0.0f;
    }
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat(("##" + label + propertyId).c_str(), &value, controlProperties.dragSpeed);
}

} // Namespace ScarlettEditor.
