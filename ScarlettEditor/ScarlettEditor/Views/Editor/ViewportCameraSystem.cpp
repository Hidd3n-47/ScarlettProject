#include "ScarlettEditorPch.h"
#include "ViewportCameraSystem.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/ext/matrix_transform.hpp>

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

    ScarlEntt::EntityHandle entity{entityIds[0], mSceneRef };
    ScarlettGame::Transform* transform = entity.GetComponent<ScarlettGame::Transform>();

    transform->translation.z = -2.0f;
    //todo add error checking to ensure the correct number of [viewport] camera's exist.
    //todo fix as the look at at translation is completely incorrect.
    viewportCamera[0].viewMatrix = glm::lookAt(transform->translation, glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    viewportCamera[0].projectionMatrix = glm::perspective(glm::radians(60.0f), viewportCamera[0].viewportWidth / viewportCamera[0].viewportHeight, 0.1f, 100.0f);
}

} // Namespace ScarlettEditor.
