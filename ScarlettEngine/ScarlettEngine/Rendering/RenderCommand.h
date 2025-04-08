#pragma once

#include <ScarlEntt/ComponentRef.h>

#include <ScarlettGameCore/Components/Transform.h>

namespace Scarlett
{

enum class RenderType
{
    SPRITE
};

class RenderCommand
{
public:
    RenderCommand() = default;

    RenderCommand(const ScarlettMath::Vec4 color, const ScarlEntt::ComponentRef<ScarlettGame::Transform> transform)
        : color(color), transform(transform)
    { /* Empty. */ }

    ScarlettMath::Vec4 color;
    ScarlEntt::ComponentRef<ScarlettGame::Transform> transform;
};

}
