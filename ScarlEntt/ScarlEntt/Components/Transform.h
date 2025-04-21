#pragma once

#include <Math/Quat.h>

#include "ScarlEntt/Debug.h"

namespace Scarlett::Component
{

struct Transform
{
    ScarlettMath::Vec3 translation   { 0.0f };
    ScarlettMath::Quat rotation      { ScarlettMath::Quat::Identity() };
    ScarlettMath::Vec3 scale         { 1.0f };

    COMPONENT_SERIALIZATION(
        { "position", "1,1,1" },
        { "rotation", "1,1,1,1" },
        { "scale", "1,1,1" })
};

} // Namespace Scarlett::Component.
