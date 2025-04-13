#pragma once

#include <ScarlEntt/ISystem.h>

namespace Scarlett
{

class SquareSpriteSystem final : public ScarlEntt::ISystem
{
public:
    SquareSpriteSystem(ScarlEntt::Scene* sceneRef, ScarlEntt::ComponentManager* componentManagerRef);

    void UpdateSystem() override;
};

} // Scarlett.
