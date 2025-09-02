#pragma once

#include <ScarlEntt/ComponentRef.h>

#include <Components/Transform.h>

#include <Components/Material.h>

namespace Scarlett
{

class RenderCommand
{
public:
    RenderCommand() = default;

    RenderCommand(const ScarlettMath::Vec4 color, const ScarlEntt::ComponentRef<Component::Transform> transform, const Material& material)
        : color(color), transform(transform), material(material)
    { /* Empty. */ }

    ScarlettMath::Vec4 color;
    ScarlEntt::ComponentRef<Component::Transform> transform;
    Material material;
};

}
