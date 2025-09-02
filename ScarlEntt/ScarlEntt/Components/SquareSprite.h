#pragma once

#include <Math/Math.h>

#include "ScarlEntt/Debug.h"

#include "Material.h"

namespace Scarlett::Component
{

struct SquareSprite
{
    ScarlettMath::Vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
    Material material;

    SCARLETT_COMPONENT(SquareSprite)
};

} // Namespace Scarlett::Component.
