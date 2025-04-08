#pragma once

#include <ScarlEntt/ISystem.h>

namespace Scarlett
{

class SquareSpriteSystem final : public ScarlEntt::ISystem
{
public:
    SquareSpriteSystem(ScarlEntt::Scene* sceneRef, ScarlEntt::ComponentManager* componentManagerRef);

    inline void InitSystem(ScarlEntt::ISystemProperties* properties) override { }
    void UpdateSystem()     override;
    inline void DestroySystem()    override { }
};

} // Scarlett.
