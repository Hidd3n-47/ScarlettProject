#pragma once

#include <ScarlEntt/ISystem.h>

#include <Rendering/Mesh.h>

namespace Scarlett
{

class SquareSpriteSystem final : public ScarlEntt::ISystem
{
public:
    SquareSpriteSystem(ScarlEntt::Scene* sceneRef, ScarlEntt::ComponentManager* componentManagerRef);

    void UpdateSystem() override;
private:
    WeakRef<Mesh> mSquareMesh;
};

} // Scarlett.
