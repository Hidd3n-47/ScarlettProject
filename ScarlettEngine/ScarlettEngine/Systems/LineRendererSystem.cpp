#include "ScarlettEnginePch.h"
#include "LineRendererSystem.h"

#include <ScarlEntt/Scene.h>

#include <Components/Line.h>

#include "Rendering/Renderer.h"

#include "Core/Engine.h"
#include "Resources/Manager/MeshManager.h"
#include "Resources/Manager/MaterialManager.h"

namespace Scarlett
{

LineRendererSystem::LineRendererSystem(ScarlEntt::Scene* sceneRef, ScarlEntt::ComponentManager* componentManagerRef)
{
    mSceneRef               = sceneRef;
    mComponentManagerRef    = componentManagerRef;

    mLineMesh = Engine::Instance().GetMeshManager()->AddMesh(Filepath{ "Assets/Mesh/CylinderLowPoly.obj"});
}

void LineRendererSystem::UpdateSystem()
{
    const auto& components = mComponentManagerRef->GetComponentArray<Component::Line>();
    const auto& entityIds = components.GetCorrespondingEntityId();

    for (ScarlEntt::ComponentId i{0}; i < components.Size(); ++i)
    {
        ScarlEntt::EntityHandle entity{entityIds[i], mSceneRef };

        const auto transform = entity.GetComponent<Component::Transform>();

        const ScarlettMath::Vec3 dVec = components[i].end - components[i].start;
        const float dVecMag = ScarlettMath::Magnitude(dVec);

        transform->translation = components[i].start;
        transform->scale = { dVecMag, 0.02f, 0.02f };
        transform->rotation = ScarlettMath::Quat::GetRotationToRotateVectorToVector({ 1.0f, 0.0f, 0.0f }, dVec / dVecMag);

        Renderer::Instance()->AddCommand(mLineMesh, { components[i].color, ScarlEntt::ComponentRef(entityIds[i], &mComponentManagerRef->GetComponentArray<Component::Transform>()), Material{ MaterialManager::DEFAULT_MATERIAL_ULID } });
    }
}


} // Namespace Scarlett.
