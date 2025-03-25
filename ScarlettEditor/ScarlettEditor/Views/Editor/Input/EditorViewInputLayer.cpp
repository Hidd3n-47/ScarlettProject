#include "ScarlettEditorPch.h"
#include "EditorViewInputLayer.h"

#include <Math/Math.h>

#include <Core/Input/KeyCodes.h>

#include <ScarlettGameCore/Src/GameCore.h>
#include <ScarlettGameCore/Components/Transform.h>

#include "Views/Editor/ViewportCamera.h"
#include "Views/Editor/View/EditorView.h"
#include "Views/Editor/Panels/ViewportPanel.h"

namespace ScarlettEditor
{

void EditorViewInputLayer::OnEvent(Scarlett::Event& e)
{
    Scarlett::EventDispatcher dispatcher(e);
    dispatcher.Dispatch<Scarlett::MouseButtonPressedEvent>(SCARLETT_BIND_FUNCTION(EditorViewInputLayer::OnMouseButtonPressed));
    dispatcher.Dispatch<Scarlett::MouseButtonReleasedEvent>(SCARLETT_BIND_FUNCTION(EditorViewInputLayer::OnMouseButtonReleased));
    dispatcher.Dispatch<Scarlett::KeyPressedEvent>(SCARLETT_BIND_FUNCTION(EditorViewInputLayer::OnKeyPressed));

    if (!e.IsHandled())
    {
        EditorInputLayer::OnEvent(e);
    }
}

bool EditorViewInputLayer::OnMouseButtonPressed(const Scarlett::MouseButtonPressedEvent& e)
{
    if (e.GetMouseButton() == Scarlett::KeyCode::MOUSE_BUTTON_2)
    {
        if (mEditorView->GetPanel<ViewportPanel>()->IsHovered())
        {
            mPanning = true;
            return true;
        }
    }

    return EditorInputLayer::OnMouseButtonPressed(e);
}

bool EditorViewInputLayer::OnMouseButtonReleased(const Scarlett::MouseButtonReleasedEvent& e)
{
    if (mPanning)
    {
        mPanning = false;
        return true;
    }

    return EditorInputLayer::OnMouseButtonReleased(e);
}

bool EditorViewInputLayer::OnKeyPressed(const Scarlett::KeyPressedEvent& e)
{
    //todo this should move into a viewport selection input manager.
    if (!mPanning)
    {
        return EditorInputLayer::OnKeyPressed(e);
    }

    ScarlettMath::Vec2 cameraMoveDir { 0.0f };

    switch (e.GetKeyCode())
    {
        case Scarlett::KeyCode::KEY_W:
            cameraMoveDir += ScarlettMath::Vec2{ 0.0f,  1.0f };
            break;
        case Scarlett::KeyCode::KEY_S:
            cameraMoveDir += ScarlettMath::Vec2{ 0.0f, -1.0f };
            break;
        case Scarlett::KeyCode::KEY_A:
            cameraMoveDir += ScarlettMath::Vec2{ -1.0f, 0.0f };
            break;
        case Scarlett::KeyCode::KEY_D:
            cameraMoveDir += ScarlettMath::Vec2{  1.0f, 0.0f };
            break;
    default:
        return EditorInputLayer::OnKeyPressed(e);
    }

    //todo this need to be improved as this cannot be the only way to get a component of an entity given a different component.
    const auto& viewportCamera = ScarlettGame::GameCore::Instance().GetActiveScene()->GetComponentManager()->GetComponentArray<ViewportCamera>();
    const auto& entityIds = viewportCamera.GetCorrespondingEntityId();

    ScarlEntt::EntityHandle entity{entityIds[0], ScarlettGame::GameCore::Instance().GetActiveScene() };
    entity.GetComponent<ScarlettGame::Transform>()->translation += ScarlettMath::Vec3{ cameraMoveDir, 0.0f };

    return true;
}

} // Namespace ScarlettEditor.
