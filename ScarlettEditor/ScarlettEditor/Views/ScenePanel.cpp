#include "ScarlettEditorPch.h"
#include "ScenePanel.h"

#include <ScarlEntt/Scene.h>

#include <ScarlettGame/Components/Tag.h>
#include <ScarlettGame/Components/Transform.h>
#include <ScarlettGame/Components/SquareSprite.h>


#include "Editor/EditorManager.h"

namespace ScarlettEditor
{

void ScenePanel::Render()
{
    ScarlEntt::ComponentManager* componentManager = EditorManager::Instance().GetCurrentScene()->GetComponentManager();
    auto& tags = componentManager->GetComponentArray<Scarlett::Tag>();

    static int addedEntities = 0;
    if (ImGui::Button("Add Entity"))
    {
        // todo fix we have a crash when components are added cross boundary.
        const auto entity = EditorManager::Instance().GetCurrentScene()->CreateEntity();
        entity.AddComponent<Scarlett::Transform>();
        entity.AddComponent<Scarlett::Tag>("Entity", entity);
        entity.AddComponent<Scarlett::SquareSprite>();
    }

    bool selectedEntity = false;
    for (ScarlEntt::ComponentId i {0}; i < tags.Size(); ++i)
    {
        if (ImGui::CollapsingHeader(tags[i].name))
        {
            selectedEntity = true;
            //mSelectionManager.SetSelectedEntity(&tags[i].entity);
        }
    }

    // if (!selectedEntity)
    // {
    //     mSelectionManager.SetSelectedEntity(nullptr);
    // }
}

} // Namespace ScarlettEditor
