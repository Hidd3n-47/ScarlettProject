#include "ScarlettEditorPch.h"
#include "EntityContextMenu.h"

#include <imgui/imgui.h>

namespace ScarlettEditor
{

void EntityContextMenu::RenderContextMenu()
{
    if (ImGui::BeginPopupContextItem("EntityContextMenu"))
    {
        float value;
        if (ImGui::Selectable("Set to zero")) value = 0.0f;
        if (ImGui::Selectable("Set to PI")) value = 3.1415f;
        ImGui::SetNextItemWidth(-FLT_MIN);
        ImGui::DragFloat("##Value", &value, 0.1f, 0.0f, 0.0f);
        ImGui::EndPopup();
    }

    ImGui::OpenPopup("EntityContextMenu");
}

} // Namespace ScarlettEditor.
