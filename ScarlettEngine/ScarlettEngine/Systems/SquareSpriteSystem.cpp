#include "ScarlettEnginePch.h"
#include "SquareSpriteSystem.h"

#include <ScarlEntt/Scene.h>

#include <Components/SquareSprite.h>

#include "Rendering/Renderer.h"

namespace Scarlett
{

SquareSpriteSystem::SquareSpriteSystem(ScarlEntt::Scene* sceneRef, ScarlEntt::ComponentManager* componentManagerRef)
{
    mSceneRef               = sceneRef;
    mComponentManagerRef    = componentManagerRef;
}

void SquareSpriteSystem::UpdateSystem()
{
    const auto& squareSprites = mComponentManagerRef->GetComponentArray<Component::SquareSprite>();
    const auto& entityIds = squareSprites.GetCorrespondingEntityId();
    for (ScarlEntt::ComponentId i{0}; i < squareSprites.Size(); ++i)
    {
        Renderer::Instance().AddCommand(RenderType::SPRITE,
            { squareSprites[i].color, ScarlEntt::ComponentRef(entityIds[i], &mComponentManagerRef->GetComponentArray<Component::Transform>()) });
    }
}

} // Namespace Scarlett
