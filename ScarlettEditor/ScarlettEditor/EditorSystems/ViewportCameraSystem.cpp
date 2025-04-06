#include "ScarlettEditorPch.h"
#include "ViewportCameraSystem.h"

#include <ScarlettGameCore/Components/Transform.h>
#include <ScarlettGameCore/Src/GameCore.h>

namespace ScarlettEditor
{

ViewportCameraSystem::ViewportCameraSystem(ScarlEntt::Scene* sceneRef, ScarlEntt::ComponentManager* componentManagerRef)
{
    mSceneRef               = sceneRef;
    mComponentManagerRef    = componentManagerRef;
}

void ViewportCameraSystem::UpdateSystem()
{
    //todo should this be templated to get the component array as this is a "mouthful" of a line.
    auto& viewportCamera = ScarlettGame::GameCore::Instance().GetActiveScene()->GetComponentManager()->GetComponentArray<ViewportCamera>();
    const auto& entityIds = viewportCamera.GetCorrespondingEntityId();

    //todo add error checking to ensure the correct number of [viewport] camera's exist.
    //todo fix as the look at at translation is completely incorrect.
    ViewportCamera* camera = &viewportCamera[0];

    ScarlEntt::EntityHandle entity{entityIds[0], mSceneRef };
    const ScarlettGame::Transform* transform = entity.GetComponent<ScarlettGame::Transform>();

    //todo implement is dirty
    if (camera->IsDirty())
    {
        // todo refactor this.
        constexpr ScarlettMath::Vec3 xAxis { 1.0f, 0.0f,  0.0f };
        constexpr ScarlettMath::Vec3 yAxis { 0.0f, 1.0f,  0.0f };
        constexpr ScarlettMath::Vec3 zAxis { 0.0f, 0.0f, -1.0f };

        camera->forwardVector   = ScarlettMath::Quat::RotatePoint(zAxis, transform->rotation);
        camera->rightVector     = ScarlettMath::Quat::RotatePoint(xAxis, transform->rotation);
        camera->upVector        = ScarlettMath::Quat::RotatePoint(yAxis, transform->rotation);

        camera->viewMatrix      = ScarlettMath::LookAt(transform->translation, transform->translation + camera->forwardVector, camera->upVector);

        camera->projectionMatrix = ScarlettMath::Perspective(60.0f, viewportCamera[0].viewportWidth / viewportCamera[0].viewportHeight, 0.1f, 100.0f);
    }

}

} // Namespace ScarlettEditor.
