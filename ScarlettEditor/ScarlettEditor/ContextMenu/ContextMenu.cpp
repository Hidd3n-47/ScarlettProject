#include "ScarlettEditorPch.h"
#include "ContextMenu.h"

#include "imgui/imgui.h"
#include "Input/EditorInputManager.h"
#include "ScarlettGameCore/Src/GameCore.h"

namespace ScarlettEditor
{

ContextMenu::ContextMenu()
{
}

void ContextMenu::OpenContextMenu()
{

    const ScarlettMath::Vec2 mousePosition = EditorInputManager::GetMousePosition();


    //ImGui::SetNextWindowPos({ mousePosition.x, mousePosition.y });

    //ImGui::Begin("foo", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

    //ImGui::SetNextWindowSize({ 200.0f, 200.0f });
    if (ImGui::BeginPopupContextWindow("foo"))
    {
        if (ImGui::BeginMenu("Add Component"))
        {
            for(const ScarlEntt::ComponentTypeId& component : ScarlettGame::GameCore::Instance().GetActiveScene()->GetComponentManager()->GetRegisteredComponentTypes())
            {
                ImGui::MenuItem(component.c_str());
            }
            ImGui::EndMenu();
        }

        ImGui::EndPopup();
    }

    ImGui::OpenPopup("foo");
}

} // Namespace ScarlettEditor.