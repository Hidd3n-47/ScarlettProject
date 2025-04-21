#pragma once

#include <Math/Math.h>

namespace Scarlett::Component
{

struct SquareSprite
{
    ScarlettMath::Vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

    COMPONENT_SERIALIZATION({ "color", "1,1,1,1" })
};

} // Namespace Scarlett::Component.
