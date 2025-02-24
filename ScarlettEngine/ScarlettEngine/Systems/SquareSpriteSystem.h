#pragma once

#include <ScarlEntt/ISystem.h>

namespace Scarlett
{

class Device;
class Mesh;

struct SquareSpriteSystemProperties : public ScarlEntt::ISystemProperties
{
    Device* device;
};

class SquareSpriteSystem final : public ScarlEntt::ISystem
{
public:
    SquareSpriteSystem(ScarlEntt::Scene* sceneRef, ScarlEntt::ComponentManager* componentManagerRef);

    inline void InitSystem(ScarlEntt::ISystemProperties* properties) override;
    void UpdateSystem()     override;
    void DestroySystem()    override;

private:
    Mesh* mTriOne = nullptr;
    Mesh* mTriTwo = nullptr;
};

} // Scarlett.
