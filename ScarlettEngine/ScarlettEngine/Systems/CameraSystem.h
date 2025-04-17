#pragma once

#include <ScarlEntt/ISystem.h>

namespace Scarlett
{

class CameraSystem final : public ScarlEntt::ISystem
{
public:
    CameraSystem(ScarlEntt::Scene* sceneRef, ScarlEntt::ComponentManager* componentManagerRef);

    void UpdateSystem()     override;
};

} // Namespace Scarlett.
