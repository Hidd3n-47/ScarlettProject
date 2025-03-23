#include "ScarlettEditorPch.h"
#include "ScenePanel.h"

#include <ScarlEntt/Scene.h>

#include <ScarlettGameCore/Src/GameCore.h>
#include <ScarlettGameCore/Components/Tag.h>

#include "Editor/EditorManager.h"
#include "Views/Editor/View/EditorView.h"

namespace ScarlettEditor
{

void ScenePanel::Render()
{
    ScarlEntt::ComponentManager* componentManager = ScarlettGame::GameCore::Instance().GetActiveScene()->GetComponentManager();
    auto& tags = componentManager->GetComponentArray<ScarlettGame::Tag>();

    if (ImGui::Button("Add Entity"))
    {
        auto ent = ScarlettGame::GameCore::Instance().CreateEntity();
    }

    bool selectedEntity = false;
    for (ScarlEntt::ComponentId i {0}; i < tags.Size(); ++i)
    {
        if (ImGui::CollapsingHeader(tags[i].name.c_str()))
        {
            selectedEntity = true;

        	dynamic_cast<EditorView*>(mView)->GetSelectionManager().SetSelectedEntity(&tags[i].entity);
        }
    }

    if (!selectedEntity)
    {
        dynamic_cast<EditorView*>(mView)->GetSelectionManager().SetSelectedEntity(nullptr);
    }
}

} // Namespace ScarlettEditor
