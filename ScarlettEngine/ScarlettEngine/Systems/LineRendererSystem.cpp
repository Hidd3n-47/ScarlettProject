#include "ScarlettEnginePch.h"
#include "LineRendererSystem.h"

#include <Math/Trig.h>

#include <ScarlEntt/Scene.h>

#include <ScarlettEditor/EditorComponents/Line.h>

#include "Rendering/Renderer.h"

namespace Scarlett
{

LineRendererSystem::LineRendererSystem(ScarlEntt::Scene* sceneRef, ScarlEntt::ComponentManager* componentManagerRef)
{
    mSceneRef               = sceneRef;
    mComponentManagerRef    = componentManagerRef;
}

void LineRendererSystem::UpdateSystem()
{
    const auto& components = mComponentManagerRef->GetComponentArray<ScarlettEditor::Line>();
    const auto& entityIds = components.GetCorrespondingEntityId();

    for (ScarlEntt::ComponentId i{0}; i < components.Size(); ++i)
    {
        ScarlEntt::EntityHandle entity{entityIds[i], mSceneRef };

        ScarlettGame::Transform* transform = entity.GetComponent<ScarlettGame::Transform>();

        const ScarlettMath::Vec3 dVec = components[i].end - components[i].start;
        const float dVecMag = ScarlettMath::Magnitude(dVec);

        transform->translation = components[i].start;
        transform->scale = { dVecMag, 0.02f, 0.02f };
        transform->rotation = ScarlettMath::Quat::GetRotationToRotateVectorToVector({ 1.0f, 0.0f, 0.0f }, dVec / dVecMag);

        Renderer::Instance().AddCommand(RenderType::LINE,
        { components[i].color, ScarlEntt::ComponentRef(entityIds[i], &mComponentManagerRef->GetComponentArray<ScarlettGame::Transform>()) });
    }
}


} // Namespace Scarlett.
