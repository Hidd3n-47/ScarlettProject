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

    SCARLETT_COMPONENT(Transform)
};

} // Namespace Scarlett::Component.
