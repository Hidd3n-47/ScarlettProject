#include "ScarlettEnginePch.h"
#include "CameraSystem.h"

#include <ScarlettGameCore/Src/GameCore.h>

#include <Components/Camera.h>
#include <Components/Transform.h>

namespace Scarlett
{

CameraSystem::CameraSystem(ScarlEntt::Scene* sceneRef, ScarlEntt::ComponentManager* componentManagerRef)
{
    mSceneRef               = sceneRef;
    mComponentManagerRef    = componentManagerRef;
}

void CameraSystem::UpdateSystem()
{
    auto& cameraArray = ScarlettGame::GameCore::Instance().GetActiveScene()->GetComponentManager()->GetComponentArray<Scarlett::Component::Camera>();
    const auto& entityIds = cameraArray.GetCorrespondingEntityId();

    Component::Camera* camera = &cameraArray[0];

    ScarlEntt::EntityHandle entity{entityIds[0], mSceneRef };
    const ScarlEntt::ComponentRef<Component::Transform> transform = entity.GetComponent<Component::Transform>();

    if (camera->IsDirty())
    {
        constexpr ScarlettMath::Vec3 xAxis { 1.0f, 0.0f,  0.0f };
        constexpr ScarlettMath::Vec3 yAxis { 0.0f, 1.0f,  0.0f };
        constexpr ScarlettMath::Vec3 zAxis { 0.0f, 0.0f, -1.0f };

        camera->SetForwardVector(ScarlettMath::Quat::RotatePoint(zAxis, transform->rotation));
        camera->SetRightVector(ScarlettMath::Quat::RotatePoint(xAxis, transform->rotation));
        camera->SetUpVector(ScarlettMath::Quat::RotatePoint(yAxis, transform->rotation));

        camera->UpdateViewAndProjectionMatrix(transform->translation);
    }
}

} // Namespace ScarlettEditor.
