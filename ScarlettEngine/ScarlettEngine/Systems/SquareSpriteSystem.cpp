#include "ScarlettEnginePch.h"
#include "SquareSpriteSystem.h"

#include <ScarlEntt/Scene.h>

#include <ScarlettGameCore/Components/SquareSprite.h>

#include "Rendering/Renderer.h"
#include "Rendering/Commands/SpriteCommand.h"

namespace Scarlett
{

SquareSpriteSystem::SquareSpriteSystem(ScarlEntt::Scene* sceneRef, ScarlEntt::ComponentManager* componentManagerRef)
{
    mSceneRef               = sceneRef;
    mComponentManagerRef    = componentManagerRef;
}

void SquareSpriteSystem::UpdateSystem()
{
    const auto& squareSprites = mComponentManagerRef->GetComponentArray<ScarlettGame::SquareSprite>();
    const auto& entityIds = squareSprites.GetCorrespondingEntityId();
    for (ScarlEntt::ComponentId i{0}; i < squareSprites.Size(); ++i)
    {
        // todo pool the commands to not hammer the new
        // todo change to use the polymorphism
        RenderCommand* command = new RenderCommand { squareSprites[i].color, ScarlEntt::ComponentRef(entityIds[i], &mComponentManagerRef->GetComponentArray<ScarlettGame::Transform>()) };
        Renderer::Instance().AddCommand(RenderType::SPRITE, command);

    }
}

} // Namespace Scarlett
