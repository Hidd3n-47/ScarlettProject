#include "ScarlettEditorPch.h"
#include "EditorViewInputLayer.h"

#include <Math/Math.h>

#include <Core/Input/KeyCodes.h>

#include <ScarlettGameCore/Src/GameCore.h>
#include <ScarlettGameCore/Components/Transform.h>
#include <ScarlettGameCore/Components/BoundingBox.h>

#include "Views/Editor/ViewportCamera.h"
#include "Views/Editor/View/EditorView.h"
#include "Views/Editor/Panels/ViewportPanel.h"

namespace ScarlettEditor
{

bool EditorViewInputLayer::OnMouseButtonPressed(const Scarlett::MouseButtonPressedEvent& e)
{
    if (!mEditorView->GetPanel<ViewportPanel>()->IsHovered())
    {
        return EditorInputLayer::OnMouseButtonPressed(e);
    }

    if (e.GetMouseButton() == Scarlett::KeyCode::MOUSE_BUTTON_1)
    {
        ScarlEntt::Scene* scene = ScarlettGame::GameCore::Instance().GetActiveScene();
        const auto& bb = scene->GetComponentManager()->GetComponentArray<ScarlettGame::BoundingBox>();
        const auto& entityIds = bb.GetCorrespondingEntityId();

        // todo, again, need a better way.
        const auto& cameras = scene->GetComponentManager()->GetComponentArray<ViewportCamera>();
        ScarlEntt::EntityHandle cameraEntity{ cameras.GetCorrespondingEntityId()[0], ScarlettGame::GameCore::Instance().GetActiveScene() };

        const ScarlettMath::Vec3 cameraCenter = cameraEntity.GetComponent<ScarlettGame::Transform>()->translation;
        const ScarlettMath::Vec3 cameraDirection = cameras[0].forwardVector;

        vector<std::pair<ScarlEntt::EntityHandle*, ScarlettMath::Vec3>> collidedWithEntities;

        // need to make sure that it selects only the first instance (closest).
        for (ScarlEntt::ComponentId i{ 0 }; i < bb.Size(); ++i)
        {
            ScarlEntt::EntityHandle entity{ entityIds[i], ScarlettGame::GameCore::Instance().GetActiveScene() };
            const ScarlettGame::Transform* transform = entity.GetComponent<ScarlettGame::Transform>();

            const ScarlettMath::Vec3 boundingBoxCenter = bb[i].GetCenter();

            const float tMin = ( cameraDirection.x * (boundingBoxCenter.x - cameraCenter.x) + cameraDirection.y * (boundingBoxCenter.y - cameraCenter.y) + cameraDirection.z * (boundingBoxCenter.z - cameraCenter.z) )
                                / (cameraDirection.x * cameraDirection.x + cameraDirection.y * cameraDirection.y + cameraDirection.z * cameraDirection.z);

            const ScarlettMath::Vec3 closestPoint = cameraCenter + tMin * cameraDirection;

            const ScarlettMath::Vec3 rotatedMin = ScarlettMath::Rotate(bb[i].localMinimum, transform->rotation);
            const ScarlettMath::Vec3 rotatedMax = ScarlettMath::Rotate(bb[i].localMaximum, transform->rotation);

            if ((rotatedMin.x <= closestPoint.x && closestPoint.x <= rotatedMax.x) &&
                (rotatedMin.y <= closestPoint.y && closestPoint.y <= rotatedMax.y) &&
                (rotatedMin.z <= closestPoint.z && closestPoint.z <= rotatedMax.z))
            {
                collidedWithEntities.emplace_back(&entity, boundingBoxCenter);
            }
        }

        float minDistance = std::numeric_limits<float>::max();
        ScarlEntt::EntityHandle* closestEntity = nullptr;
        for (const auto& [ent, boundingBoxCenter] : collidedWithEntities)
        {
            const float distance = glm::length(boundingBoxCenter - cameraCenter);
            if (distance < minDistance)
            {
                minDistance = distance;
                closestEntity = ent;
            }
        }

        mEditorView->GetSelectionManager().SetSelectedEntity(closestEntity);
        return true;
    }

    if (e.GetMouseButton() == Scarlett::KeyCode::MOUSE_BUTTON_2)
    {
        mCameraFlying = true;
        return true;
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
        ScarlEntt::EntityHandle entity{ entityIds[0], ScarlettGame::GameCore::Instance().GetActiveScene() };
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
