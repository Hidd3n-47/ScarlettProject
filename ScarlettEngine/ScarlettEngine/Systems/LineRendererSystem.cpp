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

        const ScarlettMath::Vec3 lineVec = components[i].end - components[i].start;
        const float hypot = ScarlettMath::Magnitude(lineVec);

        const float roll    = ScarlettMath::Trig::Asin(lineVec.x / hypot);
        const float pitch   = ScarlettMath::Radians(90) - ScarlettMath::Trig::Asin(lineVec.y / hypot);

        transform->translation = lineVec * 0.5f + components[i].start;
        transform->scale = { 0.02f, hypot, 0.02f };
        transform->rotation.SetYawPitchRoll(0.0f, pitch, roll);

        Renderer::Instance().AddCommand(RenderType::LINE,
        { components[i].color, ScarlEntt::ComponentRef(entityIds[i], &mComponentManagerRef->GetComponentArray<ScarlettGame::Transform>()) });
    }
}


} // Namespace Scarlett.
