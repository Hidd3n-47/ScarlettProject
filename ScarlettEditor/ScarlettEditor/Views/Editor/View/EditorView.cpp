#include "ScarlettEditorPch.h"
#include "EditorView.h"

#include <ScarlEntt/Scene.h>

#include <ScarlettGameCore/Src/GameCore.h>

#include <Components/Transform.h>
#include <Components/SquareSprite.h>

#include "Serialization/SceneSerialization.h"

#include "Editor/EditorManager.h"

#include "Views/Editor/Panels/ScenePanel.h"
#include "Views/Editor/Panels/ViewportPanel.h"
#include "Views/Editor/Panels/PropertiesPanel.h"


#include "Views/Editor/Input/EditorViewInputLayer.h"

#include <Components/Line.h>

namespace ScarlettEditor
{

EditorView::EditorView()
{
    SceneSerialization::DeserializeCurrentGameScene();

    //todo Add blending for the grid.
    ScarlEntt::EntityHandle viewportGrid = ScarlettGame::GameCore::Instance().CreateEntity();
    viewportGrid.GetComponent<Scarlett::Component::Transform>()->translation = { 0.0f, -0.5f, 0.0f };
    viewportGrid.GetComponent<Scarlett::Component::Transform>()->rotation = ScarlettMath::Quat { 0.0f, ScarlettMath::Radians(90.0f), 0.0f };
    viewportGrid.GetComponent<Scarlett::Component::Transform>()->scale = { 100.0f, 100.0f, 0.0f };
    viewportGrid.GetComponent<Scarlett::Component::SquareSprite>()->color = { 1.0f, 1.0f, 1.0f, 0.2f };

    // Todo remove this once debugging has been confirmed.
    // Cube Wireframe.
    {
        const ScarlEntt::EntityHandle line = ScarlettGame::GameCore::Instance().CreateEntity();
        line.RemoveComponent<Scarlett::Component::SquareSprite>();
        line.AddComponent<Scarlett::Component::Line>(ScarlettMath::Vec3{ -0.5f, 0.5f, 0.5f }, ScarlettMath::Vec3{ 0.5f, 0.5f, 0.5f }, ScarlettMath::Vec4{ 0.16f, 0.78f, 1.0f, 1.0f });
    }
    {
        const ScarlEntt::EntityHandle line = ScarlettGame::GameCore::Instance().CreateEntity();
        line.RemoveComponent<Scarlett::Component::SquareSprite>();
        line.AddComponent<Scarlett::Component::Line>(ScarlettMath::Vec3{ -0.5f, -0.5f, 0.5f }, ScarlettMath::Vec3{ 0.5f, -0.5f, 0.5f }, ScarlettMath::Vec4{ 0.16f, 0.78f, 1.0f, 1.0f });
    }
    {
        const ScarlEntt::EntityHandle line = ScarlettGame::GameCore::Instance().CreateEntity();
        line.RemoveComponent<Scarlett::Component::SquareSprite>();
        line.AddComponent<Scarlett::Component::Line>(ScarlettMath::Vec3{ -0.5f, -0.5f, 0.5f }, ScarlettMath::Vec3{ -0.5f, 0.5f, 0.5f }, ScarlettMath::Vec4{ 0.16f, 0.78f, 1.0f, 1.0f });
    }
    {
        const ScarlEntt::EntityHandle line = ScarlettGame::GameCore::Instance().CreateEntity();
        line.RemoveComponent<Scarlett::Component::SquareSprite>();
        line.AddComponent<Scarlett::Component::Line>(ScarlettMath::Vec3{ 0.5f, -0.5f, 0.5f }, ScarlettMath::Vec3{ 0.5f, 0.5f, 0.5f }, ScarlettMath::Vec4{ 0.16f, 0.78f, 1.0f, 1.0f });
    }
    {
        const ScarlEntt::EntityHandle line = ScarlettGame::GameCore::Instance().CreateEntity();
        line.RemoveComponent<Scarlett::Component::SquareSprite>();
        line.AddComponent<Scarlett::Component::Line>(ScarlettMath::Vec3{ -0.5f, 0.5f, 0.5f }, ScarlettMath::Vec3{ -0.5f, 0.5f, -0.5f }, ScarlettMath::Vec4{ 0.16f, 0.78f, 1.0f, 1.0f });
    }
    {
        const ScarlEntt::EntityHandle line = ScarlettGame::GameCore::Instance().CreateEntity();
        line.RemoveComponent<Scarlett::Component::SquareSprite>();
        line.AddComponent<Scarlett::Component::Line>(ScarlettMath::Vec3{ -0.5f, -0.5f, 0.5f }, ScarlettMath::Vec3{ -0.5f, -0.5f, -0.5f }, ScarlettMath::Vec4{ 0.16f, 0.78f, 1.0f, 1.0f });
    }
    {
        const ScarlEntt::EntityHandle line = ScarlettGame::GameCore::Instance().CreateEntity();
        line.RemoveComponent<Scarlett::Component::SquareSprite>();
        line.AddComponent<Scarlett::Component::Line>(ScarlettMath::Vec3{ -0.5f, -0.5f, -0.5f }, ScarlettMath::Vec3{ -0.5f, 0.5f, -0.5f }, ScarlettMath::Vec4{ 0.16f, 0.78f, 1.0f, 1.0f });
    }
    {
        const ScarlEntt::EntityHandle line = ScarlettGame::GameCore::Instance().CreateEntity();
        line.RemoveComponent<Scarlett::Component::SquareSprite>();
        line.AddComponent<Scarlett::Component::Line>(ScarlettMath::Vec3{ 0.5f, 0.5f, -0.5f }, ScarlettMath::Vec3{ 0.5f, 0.5f, 0.5f }, ScarlettMath::Vec4{ 0.16f, 0.78f, 1.0f, 1.0f });
    }
    {
        const ScarlEntt::EntityHandle line = ScarlettGame::GameCore::Instance().CreateEntity();
        line.RemoveComponent<Scarlett::Component::SquareSprite>();
        line.AddComponent<Scarlett::Component::Line>(ScarlettMath::Vec3{ 0.5f, -0.5f, -0.5f }, ScarlettMath::Vec3{ 0.5f, -0.5f, 0.5f }, ScarlettMath::Vec4{ 0.16f, 0.78f, 1.0f, 1.0f });
    }
    {
        const ScarlEntt::EntityHandle line = ScarlettGame::GameCore::Instance().CreateEntity();
        line.RemoveComponent<Scarlett::Component::SquareSprite>();
        line.AddComponent<Scarlett::Component::Line>(ScarlettMath::Vec3{ 0.5f, 0.5f, -0.5f }, ScarlettMath::Vec3{ 0.5f, -0.5f, -0.5f }, ScarlettMath::Vec4{ 0.16f, 0.78f, 1.0f, 1.0f });
    }
    {
        const ScarlEntt::EntityHandle line = ScarlettGame::GameCore::Instance().CreateEntity();
        line.RemoveComponent<Scarlett::Component::SquareSprite>();
        line.AddComponent<Scarlett::Component::Line>(ScarlettMath::Vec3{ 0.5f, 0.5f, -0.5f }, ScarlettMath::Vec3{ -0.5f, 0.5f, -0.5f }, ScarlettMath::Vec4{ 0.16f, 0.78f, 1.0f, 1.0f });
    }
    {
        const ScarlEntt::EntityHandle line = ScarlettGame::GameCore::Instance().CreateEntity();
        line.RemoveComponent<Scarlett::Component::SquareSprite>();
        line.AddComponent<Scarlett::Component::Line>(ScarlettMath::Vec3{ 0.5f, -0.5f, -0.5f }, ScarlettMath::Vec3{ -0.5f, -0.5f, -0.5f }, ScarlettMath::Vec4{ 0.16f, 0.78f, 1.0f, 1.0f });
    }

    // Cube diagonals.
    {
        const ScarlEntt::EntityHandle line = ScarlettGame::GameCore::Instance().CreateEntity();
        line.RemoveComponent<Scarlett::Component::SquareSprite>();
        line.AddComponent<Scarlett::Component::Line>(ScarlettMath::Vec3{ 0.5f, 0.5f, 0.5f }, ScarlettMath::Vec3{ -0.5f, -0.5f, 0.5f }, ScarlettMath::Vec4{ 0.71f, 0.71f, 0.06f, 1.0f });
    }
    {
        const ScarlEntt::EntityHandle line = ScarlettGame::GameCore::Instance().CreateEntity();
        line.RemoveComponent<Scarlett::Component::SquareSprite>();
        line.AddComponent<Scarlett::Component::Line>(ScarlettMath::Vec3{ -0.5f, 0.5f, 0.5f }, ScarlettMath::Vec3{ 0.5f, -0.5f, 0.5f }, ScarlettMath::Vec4{ 0.71f, 0.71f, 0.06f, 1.0f });
    }
    {
        const ScarlEntt::EntityHandle line = ScarlettGame::GameCore::Instance().CreateEntity();
        line.RemoveComponent<Scarlett::Component::SquareSprite>();
        line.AddComponent<Scarlett::Component::Line>(ScarlettMath::Vec3{ -0.5f, -0.5f, -0.5f }, ScarlettMath::Vec3{ -0.5f, 0.5f, 0.5f }, ScarlettMath::Vec4{ 0.71f, 0.71f, 0.06f, 1.0f });
    }
    {
        const ScarlEntt::EntityHandle line = ScarlettGame::GameCore::Instance().CreateEntity();
        line.RemoveComponent<Scarlett::Component::SquareSprite>();
        line.AddComponent<Scarlett::Component::Line>(ScarlettMath::Vec3{ -0.5f, -0.5f, 0.5f }, ScarlettMath::Vec3{ -0.5f, 0.5f, -0.5f }, ScarlettMath::Vec4{ 0.71f, 0.71f, 0.06f, 1.0f });
    }
    {
        const ScarlEntt::EntityHandle line = ScarlettGame::GameCore::Instance().CreateEntity();
        line.RemoveComponent<Scarlett::Component::SquareSprite>();
        line.AddComponent<Scarlett::Component::Line>(ScarlettMath::Vec3{ 0.5f, 0.5f, 0.5f }, ScarlettMath::Vec3{ 0.5f, -0.5f, -0.5f }, ScarlettMath::Vec4{ 0.71f, 0.71f, 0.06f, 1.0f });
    }
    {
        const ScarlEntt::EntityHandle line = ScarlettGame::GameCore::Instance().CreateEntity();
        line.RemoveComponent<Scarlett::Component::SquareSprite>();
        line.AddComponent<Scarlett::Component::Line>(ScarlettMath::Vec3{ 0.5f, 0.5f, -0.5f }, ScarlettMath::Vec3{ 0.5f, -0.5f, 0.5f }, ScarlettMath::Vec4{ 0.71f, 0.71f, 0.06f, 1.0f });
    }

    // Cube internal diagonals
    {
        const ScarlEntt::EntityHandle line = ScarlettGame::GameCore::Instance().CreateEntity();
        line.RemoveComponent<Scarlett::Component::SquareSprite>();
        line.AddComponent<Scarlett::Component::Line>(ScarlettMath::Vec3{ 0.5f, 0.5f, 0.5f }, ScarlettMath::Vec3{ -0.0f, -0.0f, -0.0f }, ScarlettMath::Vec4{ 0.65f, 0.12f, 0.78f, 1.0f });
    }
    {
        const ScarlEntt::EntityHandle line = ScarlettGame::GameCore::Instance().CreateEntity();
        line.RemoveComponent<Scarlett::Component::SquareSprite>();
        line.AddComponent<Scarlett::Component::Line>(ScarlettMath::Vec3{ -0.5f, 0.5f, 0.5f }, ScarlettMath::Vec3{ 0.5f, -0.5f, -0.5f }, ScarlettMath::Vec4{ 0.65f, 0.12f, 0.78f, 1.0f });
    }
    {
        const ScarlEntt::EntityHandle line = ScarlettGame::GameCore::Instance().CreateEntity();
        line.RemoveComponent<Scarlett::Component::SquareSprite>();
        line.AddComponent<Scarlett::Component::Line>(ScarlettMath::Vec3{ -0.5f, 0.0f, 0.0f }, ScarlettMath::Vec3{ 0.5f, 0.0f, 0.0f }, ScarlettMath::Vec4{ 0.65f, 0.12f, 0.78f, 1.0f });
    }

    {
        const ScarlEntt::EntityHandle line = ScarlettGame::GameCore::Instance().CreateEntity();
        line.RemoveComponent<Scarlett::Component::SquareSprite>();
        line.AddComponent<Scarlett::Component::Line>(ScarlettMath::Vec3{ 2.0f, 2.0f, 2.0f }, ScarlettMath::Vec3{ 0.5f, 0.5f, 0.5f }, ScarlettMath::Vec4{ 1.0f, 1.0f, 0.0f, 1.0f });
    }
    {
        const ScarlEntt::EntityHandle line = ScarlettGame::GameCore::Instance().CreateEntity();
        line.RemoveComponent<Scarlett::Component::SquareSprite>();
        line.AddComponent<Scarlett::Component::Line>(ScarlettMath::Vec3{ -2.0f, 2.0f, 2.0f }, ScarlettMath::Vec3{ -0.5f, 0.5f, 0.5f }, ScarlettMath::Vec4{ 1.0f, 1.0f, 0.0f, 1.0f });
    }

    AddPanel<ScenePanel>();
    AddPanel<ViewportPanel>();
    AddPanel<PropertiesPanel>();

    mEditorViewOverlay = EditorManager::Instance().GetLayerStack()->PushOverlay<EditorViewInputLayer>(this);
}

EditorView::~EditorView()
{
    SceneSerialization::SerializeCurrentGameScene();

    EditorManager::Instance().GetLayerStack()->PopOverlay(mEditorViewOverlay);
}

} // ScarlettEditor.


