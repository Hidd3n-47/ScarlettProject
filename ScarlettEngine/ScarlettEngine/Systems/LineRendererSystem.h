#pragma once

#include <ScarlEntt/ISystem.h>

namespace Scarlett
{

class LineRendererSystem final : public ScarlEntt::ISystem
{
public:
    LineRendererSystem(ScarlEntt::Scene* sceneRef, ScarlEntt::ComponentManager* componentManagerRef);

    void UpdateSystem() override;
};

} // Namespace Scarlett.
