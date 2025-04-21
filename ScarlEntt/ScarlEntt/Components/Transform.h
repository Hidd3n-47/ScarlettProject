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
        { "position" , SerializationUtils::ToString(translation) },
        { "rotation" , SerializationUtils::ToString(rotation) },
        { "scale"    , SerializationUtils::ToString(scale) })
};

} // Namespace Scarlett::Component.
