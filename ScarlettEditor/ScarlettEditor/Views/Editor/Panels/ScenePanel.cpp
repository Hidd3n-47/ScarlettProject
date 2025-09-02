#include "ScarlettEditorPch.h"
#include "ScenePanel.h"

#include <ScarlEntt/Scene.h>

#include <ScarlettGameCore/Src/GameCore.h>

#include <Components/Tag.h>

#include "Views/Editor/View/EditorView.h"

namespace ScarlettEditor
{

void ScenePanel::Render()
{
    EditorView* editorView = dynamic_cast<EditorView*>(mView);

    ScarlEntt::ComponentManager* componentManager = ScarlettGame::GameCore::Instance().GetActiveScene()->GetComponentManager();
    auto& tags = componentManager->GetComponentArray<Scarlett::Component::Tag>();

    // ----------- Title Bar ----------------
    ImGui::BeginGroup();

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.0f);
    ImGui::Text("Default Scene Name");
    ImGui::SameLine();
    ImGui::SetCursorPos({ ImGui::GetWindowWidth()  - 35.0f, ImGui::GetCursorPosY() - 5.0f });
    if (ImGui::Button("+"))
    {
        (void)ScarlettGame::GameCore::Instance().CreateEntity();
    }
    ImGui::EndGroup();

    ImGui::Separator();

    // ------------- Tree -------------------
    const float indentSpacing = ImGui::GetStyle().IndentSpacing;
    ImGui::GetStyle().IndentSpacing = 0.0f;

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
    ImGui::TreePush("Scene");

    for (ScarlEntt::ComponentId i {0}; i < tags.Size(); ++i)
    {
        ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_SpanAvailWidth;

        if (editorView->GetSelectionManager().GetSelectedEntity() == &tags[i].entity)
        {
            nodeFlags |= ImGuiTreeNodeFlags_Selected;
        }

        // todo when there are child entities, enable this and have the scene graph show the children indented.
        constexpr bool hasChildEntity = false;
        if (!hasChildEntity)
        {
            nodeFlags |= ImGuiTreeNodeFlags_Leaf;
        }

        if (ImGui::TreeNodeEx(tags[i].name.c_str(), nodeFlags))
        {
            if (ImGui::IsItemClicked())
            {
                editorView->GetSelectionManager().SetSelectedEntity(&tags[i].entity);
            }

            ImGui::TreePop();
        }
    }

    ImGui::TreePop();
    ImGui::PopStyleVar();

    ImGui::GetStyle().IndentSpacing = indentSpacing;
}

} // Namespace ScarlettEditor
