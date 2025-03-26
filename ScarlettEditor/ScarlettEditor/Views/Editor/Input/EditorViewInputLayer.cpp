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
    dispatcher.Dispatch<Scarlett::MouseMovedEvent>(SCARLETT_BIND_FUNCTION(EditorViewInputLayer::OnMouseMoved));

    dispatcher.Dispatch<Scarlett::KeyPressedEvent>(SCARLETT_BIND_FUNCTION(EditorViewInputLayer::OnKeyPressed));
    dispatcher.Dispatch<Scarlett::KeyReleasedEvent>(SCARLETT_BIND_FUNCTION(EditorViewInputLayer::OnKeyReleased));

    dispatcher.Dispatch<Scarlett::OnUpdateEvent>(SCARLETT_BIND_FUNCTION(EditorViewInputLayer::OnUpdateEvent));

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
            mCameraFlying = true;
            return true;
        }
    }

    return EditorInputLayer::OnMouseButtonPressed(e);
}

bool EditorViewInputLayer::OnMouseButtonReleased(const Scarlett::MouseButtonReleasedEvent& e)
{
    if (mCameraFlying)
    {
        mCameraFlying = false;
        return true;
    }

    return EditorInputLayer::OnMouseButtonReleased(e);
}

bool EditorViewInputLayer::OnMouseMoved(const Scarlett::MouseMovedEvent& e)
{
    const ScarlettMath::Vec2 mousePosition { e.GetXPos(), e.GetYPos() };

    if (mCameraFlying)
    {
        //todo cache camera as there will only be one.
        const auto& viewportCamera = ScarlettGame::GameCore::Instance().GetActiveScene()->GetComponentManager()->GetComponentArray<ViewportCamera>();
        const auto& entityIds = viewportCamera.GetCorrespondingEntityId();
        ScarlEntt::EntityHandle entity{entityIds[0], ScarlettGame::GameCore::Instance().GetActiveScene() };
        ScarlettGame::Transform* transform = entity.GetComponent<ScarlettGame::Transform>();

        constexpr float SPEED_SCALING_FACTOR = 0.1f;
        const ScarlettMath::Vec2 moveDelta = mousePosition - mPreviousMousePosition;

        transform->rotation.y -= moveDelta.x * SPEED_SCALING_FACTOR;
        transform->rotation.x -= moveDelta.y * SPEED_SCALING_FACTOR;
    }

    mPreviousMousePosition = mousePosition;

    return EditorInputLayer::OnMouseMoved(e);
}

bool EditorViewInputLayer::OnKeyPressed(const Scarlett::KeyPressedEvent& e)
{
    //todo this should move into a viewport selection input manager.
    if (!mCameraFlying)
    {
        return EditorInputLayer::OnKeyPressed(e);
    }

    // todo change this to be event based so that it can be smooth movement.
    switch (e.GetKeyCode())
    {
        case Scarlett::KeyCode::KEY_W:
            mCameraForwardDirection    +=  1.0f;
            break;
        case Scarlett::KeyCode::KEY_S:
            mCameraForwardDirection    += -1.0f;
            break;
        case Scarlett::KeyCode::KEY_A:
            mCameraHorizontalDirection += -1.0f;
            break;
        case Scarlett::KeyCode::KEY_D:
            mCameraHorizontalDirection +=  1.0f;
            break;
        case Scarlett::KeyCode::KEY_E:
            mCameraVerticalDirection   +=  1.0f;
            break;
        case Scarlett::KeyCode::KEY_Q:
            mCameraVerticalDirection   += -1.0f;
            break;
    default:
        return EditorInputLayer::OnKeyPressed(e);
    }

    return true;
}

bool EditorViewInputLayer::OnKeyReleased(const Scarlett::KeyReleasedEvent& e)
{
    switch (e.GetKeyCode())
    {
    case Scarlett::KeyCode::KEY_W:
        mCameraForwardDirection    -=  1.0f;
        break;
    case Scarlett::KeyCode::KEY_S:
        mCameraForwardDirection    -= -1.0f;
        break;
    case Scarlett::KeyCode::KEY_A:
        mCameraHorizontalDirection -= -1.0f;
        break;
    case Scarlett::KeyCode::KEY_D:
        mCameraHorizontalDirection -=  1.0f;
        break;
    case Scarlett::KeyCode::KEY_E:
        mCameraVerticalDirection   -=  1.0f;
        break;
    case Scarlett::KeyCode::KEY_Q:
        mCameraVerticalDirection   -= -1.0f;
        break;
    default:
        return EditorInputLayer::OnKeyReleased(e);
    }

    return true;
}

bool EditorViewInputLayer::OnUpdateEvent(const Scarlett::OnUpdateEvent& e)
{
    if (!mCameraFlying)
    {
        return EditorInputLayer::OnUpdateEvent(e);
    }

    constexpr float SPEED_SCALING_FACTOR = 0.01f;

    //todo this need to be improved as this cannot be the only way to get a component of an entity given a different component.
    const auto& viewportCameraArray = ScarlettGame::GameCore::Instance().GetActiveScene()->GetComponentManager()->GetComponentArray<ViewportCamera>();
    const auto& entityIds = viewportCameraArray.GetCorrespondingEntityId();

    ScarlEntt::EntityHandle entity{ entityIds[0], ScarlettGame::GameCore::Instance().GetActiveScene() };

    const ViewportCamera* camera = &viewportCameraArray[0];

    // Todo (CameraIssue) Should you subtract camera forward?
    entity.GetComponent<ScarlettGame::Transform>()->translation += camera->rightVector      * mCameraHorizontalDirection    * SPEED_SCALING_FACTOR
                                                                 - camera->forwardVector    * mCameraForwardDirection       * SPEED_SCALING_FACTOR
                                                                 + camera->upVector         * mCameraVerticalDirection      * SPEED_SCALING_FACTOR;

    return true;
}

} // Namespace ScarlettEditor.
