#pragma once

#include <ScarlEntt/ISystem.h>

#include "Rendering/Mesh.h"

namespace Scarlett
{

class LineRendererSystem final : public ScarlEntt::ISystem
{
public:
    LineRendererSystem(ScarlEntt::Scene* sceneRef, ScarlEntt::ComponentManager* componentManagerRef);

    void UpdateSystem() override;
private:
    WeakRef<Mesh> mLineMesh;
};

} // Namespace Scarlett.
