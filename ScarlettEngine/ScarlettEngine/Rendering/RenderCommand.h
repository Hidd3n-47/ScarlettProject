#pragma once

#include <ScarlEntt/ComponentRef.h>

#include <Components/Transform.h>

namespace Scarlett
{

enum class RenderType
{
    SPRITE,
    LINE
};

class RenderCommand
{
public:
    RenderCommand() = default;

    RenderCommand(const ScarlettMath::Vec4 color, const ScarlEntt::ComponentRef<Component::Transform> transform)
        : color(color), transform(transform)
    { /* Empty. */ }

    ScarlettMath::Vec4 color;
    ScarlEntt::ComponentRef<Component::Transform> transform;
};

}
