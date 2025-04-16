#include "ScarlettEditorPch.h"
#include "EditorViewInputLayer.h"

#include <Math/Quat.h>

#include <Core/Input/KeyCodes.h>

#include <ScarlettGameCore/Src/GameCore.h>
#include <ScarlettGameCore/Components/Transform.h>
#include <ScarlettGameCore/Components/BoundingBox.h>

#include "Input/EditorInputManager.h"

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
            const ScarlEntt::ComponentRef<ScarlettGame::Transform> transform = entity.GetComponent<ScarlettGame::Transform>();

            const ScarlettMath::Vec3 boundingBoxCenter = bb[i].GetCenter();

            const float tMin = ( cameraDirection.x * (boundingBoxCenter.x - cameraCenter.x) + cameraDirection.y * (boundingBoxCenter.y - cameraCenter.y) + cameraDirection.z * (boundingBoxCenter.z - cameraCenter.z) )
                                / (cameraDirection.x * cameraDirection.x + cameraDirection.y * cameraDirection.y + cameraDirection.z * cameraDirection.z);

            const ScarlettMath::Vec3 closestPoint = cameraCenter + tMin * cameraDirection;

            const ScarlettMath::Vec3 rotatedMin = ScarlettMath::Quat::RotatePoint(bb[i].localMinimum, transform->rotation);
            const ScarlettMath::Vec3 rotatedMax = ScarlettMath::Quat::RotatePoint(bb[i].localMaximum, transform->rotation);

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
        const ScarlEntt::ComponentRef<ScarlettGame::Transform> transform = entity.GetComponent<ScarlettGame::Transform>();

        constexpr float SPEED_SCALING_FACTOR = 0.0025f;
        const ScarlettMath::Vec2 moveDelta = mousePosition - mPreviousMousePosition;

        float yaw, pitch, roll;
        transform->rotation.GetYawPitchRoll(yaw, pitch, roll);

        yaw    -= moveDelta.x * SPEED_SCALING_FACTOR;
        pitch  -= moveDelta.y * SPEED_SCALING_FACTOR;

        transform->rotation.SetYawPitchRoll(yaw, pitch, roll);
    }

    mPreviousMousePosition = mousePosition;

    return EditorInputLayer::OnMouseMoved(e);
}

bool EditorViewInputLayer::OnMouseScrolledEvent(const Scarlett::MouseScrollEvent& e)
{
    if (mEditorView->GetPanel<ViewportPanel>()->IsHovered())
    {
        constexpr float SCROLLING_SPEED_FACTOR  = 0.1f;
        constexpr float SPEED_MULTIPLIER_MIN    = 0.1f;
        constexpr float SPEED_MULTIPLIER_MAX    = 2.0f;

        mCameraFlyingMultiplier = ScarlettMath::Clamp(mCameraFlyingMultiplier + e.GetYOffset() * SCROLLING_SPEED_FACTOR, SPEED_MULTIPLIER_MIN, SPEED_MULTIPLIER_MAX);
        EDITOR_DLOG("Camera flying speed multiplier: {0}", mCameraFlyingMultiplier);
    }

    return EditorInputLayer::OnMouseScrolledEvent(e);
}

bool EditorViewInputLayer::OnKeyPressed(const Scarlett::KeyPressedEvent& e)
{
    if (!mCameraFlying)
    {
        return EditorInputLayer::OnKeyPressed(e);
    }

    switch (e.GetKeyCode())
    {
    case Scarlett::KeyCode::KEY_W:
    case Scarlett::KeyCode::KEY_S:
    case Scarlett::KeyCode::KEY_A:
    case Scarlett::KeyCode::KEY_D:
    case Scarlett::KeyCode::KEY_E:
    case Scarlett::KeyCode::KEY_Q:
    case Scarlett::KeyCode::KEY_LEFT_SHIFT:
    case Scarlett::KeyCode::KEY_RIGHT_SHIFT:
        return true;
    default:
        return EditorInputLayer::OnKeyPressed(e);
    }
}

bool EditorViewInputLayer::OnKeyReleased(const Scarlett::KeyReleasedEvent& e)
{
    if (!mCameraFlying)
    {
        return EditorInputLayer::OnKeyReleased(e);
    }

    switch (e.GetKeyCode())
    {
    case Scarlett::KeyCode::KEY_W:
    case Scarlett::KeyCode::KEY_S:
    case Scarlett::KeyCode::KEY_A:
    case Scarlett::KeyCode::KEY_D:
    case Scarlett::KeyCode::KEY_E:
    case Scarlett::KeyCode::KEY_Q:
    case Scarlett::KeyCode::KEY_LEFT_SHIFT:
    case Scarlett::KeyCode::KEY_RIGHT_SHIFT:
        return true;
    default:
        return EditorInputLayer::OnKeyReleased(e);
    }
}

bool EditorViewInputLayer::OnUpdateEvent(const Scarlett::OnUpdateEvent& e)
{
    if (!mCameraFlying)
    {
        return EditorInputLayer::OnUpdateEvent(e);
    }

    //todo this need to be improved as this cannot be the only way to get a component of an entity given a different component.
    const auto& viewportCameraArray = ScarlettGame::GameCore::Instance().GetActiveScene()->GetComponentManager()->GetComponentArray<ViewportCamera>();
    const auto& entityIds = viewportCameraArray.GetCorrespondingEntityId();

    ScarlEntt::EntityHandle entity{ entityIds[0], ScarlettGame::GameCore::Instance().GetActiveScene() };

    const ViewportCamera* camera = &viewportCameraArray[0];

    ScarlettMath::Vec3 cameraHorizontalDirection = { }, cameraVerticalDirection = { }, cameraForwardDirection = { };

    if (EditorInputManager::IsKeyDown(Scarlett::KeyCode::KEY_W))
    {
        cameraForwardDirection += -1.0f;
    }
    if (EditorInputManager::IsKeyDown(Scarlett::KeyCode::KEY_S))
    {
        cameraForwardDirection += 1.0f;
    }
    if (EditorInputManager::IsKeyDown(Scarlett::KeyCode::KEY_A))
    {
        cameraHorizontalDirection += -1.0f;
    }
    if (EditorInputManager::IsKeyDown(Scarlett::KeyCode::KEY_D))
    {
        cameraHorizontalDirection += 1.0f;
    }
    if (EditorInputManager::IsKeyDown(Scarlett::KeyCode::KEY_E))
    {
        cameraVerticalDirection += 1.0f;
    }
    if (EditorInputManager::IsKeyDown(Scarlett::KeyCode::KEY_Q))
    {
        cameraVerticalDirection += -1.0f;
    }

    constexpr float SPEED_SCALING_FACTOR        = 0.005f;
    constexpr float SHIFT_KEY_SPEED_MULTIPLIER  = 1.5f;

    // todo change input manager to check for flags so that we can combine this into one check.
    const float shiftKeySpeedBoost = EditorInputManager::IsKeyDown(Scarlett::KeyCode::KEY_LEFT_SHIFT) ? SHIFT_KEY_SPEED_MULTIPLIER : 1.0f;
    const float totalSpeedMultiplier = SPEED_SCALING_FACTOR * shiftKeySpeedBoost * mCameraFlyingMultiplier;

    entity.GetComponent<ScarlettGame::Transform>()->translation += camera->rightVector      * cameraHorizontalDirection    * totalSpeedMultiplier
                                                                 - camera->forwardVector    * cameraForwardDirection       * totalSpeedMultiplier
                                                                 + camera->upVector         * cameraVerticalDirection      * totalSpeedMultiplier;

    return true;
}

} // Namespace ScarlettEditor.
