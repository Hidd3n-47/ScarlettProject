#include "ScarlettEditorPch.h"
#include "ViewportCameraSystem.h"

#include <Math/Math.h>

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

        //calculate forward, right and up vector.
        const float cX = ScarlettMath::Cos(ScarlettMath::Radians(transform->rotation.x));
        const float sX = ScarlettMath::Sin(ScarlettMath::Radians(transform->rotation.x));

        const float cY = ScarlettMath::Cos(ScarlettMath::Radians(transform->rotation.y));
        const float sY = ScarlettMath::Sin(ScarlettMath::Radians(transform->rotation.y));

        const float cZ = ScarlettMath::Cos(ScarlettMath::Radians(transform->rotation.z));
        const float sZ = ScarlettMath::Sin(ScarlettMath::Radians(transform->rotation.z));

        constexpr ScarlettMath::Vec3 xAxis { 1.0f, 0.0f, 0.0f };
        constexpr ScarlettMath::Vec3 yAxis { 0.0f, 1.0f, 0.0f };
        constexpr ScarlettMath::Vec3 zAxis { 0.0f, 0.0f, -1.0f };

        const ScarlettMath::Mat3 rotateX{ 1.0f, 0.0f, 0.0f, 0.0f, cX, -sX, 0.0f, sX, cX };
        const ScarlettMath::Mat3 rotateY{ cY, 0.0f, sY, 0.0f, 1.0f, 0.0f, -sY, 0.0f, cY };
        const ScarlettMath::Mat3 rotateZ{ cZ, -sZ, 0.0f, sZ, cZ, 0.0f, 0.0f, 0.0f, 1.0f };

        const ScarlettMath::Mat3 rotation = rotateZ * rotateY * rotateX;
        camera->forwardVector   = rotation * zAxis;
        camera->rightVector     = rotation * xAxis;
        camera->upVector        = rotation * yAxis;

        // update view and proj matrix.
        const ScarlettMath::Vec3 w { ScarlettMath::Normalize(camera->forwardVector) };
        const ScarlettMath::Vec3 u { ScarlettMath::Normalize(ScarlettMath::Cross(w, camera->upVector)) };
        const ScarlettMath::Vec3 v { ScarlettMath::Cross(w, u) };

        camera->viewMatrix[0][0] = u.x;
        camera->viewMatrix[1][0] = u.y;
        camera->viewMatrix[2][0] = u.z;
        camera->viewMatrix[0][1] = v.x;
        camera->viewMatrix[1][1] = v.y;
        camera->viewMatrix[2][1] = v.z;
        camera->viewMatrix[0][2] = w.x;
        camera->viewMatrix[1][2] = w.y;
        camera->viewMatrix[2][2] = w.z;
        camera->viewMatrix[3][0] = -ScarlettMath::Dot(u, transform->translation);
        camera->viewMatrix[3][1] = -ScarlettMath::Dot(v, transform->translation);
        camera->viewMatrix[3][2] = -ScarlettMath::Dot(w, transform->translation);
    }

    camera->projectionMatrix = ScarlettMath::Perspective(60.0f, viewportCamera[0].viewportWidth / viewportCamera[0].viewportHeight, 0.1f, 100.0f);
}

} // Namespace ScarlettEditor.
