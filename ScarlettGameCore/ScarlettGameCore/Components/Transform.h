#pragma once

#include <Math/Math.h>

namespace ScarlettGame
{

struct Transform
{
    ScarlettMath::Vec3 translation   { 0.0f };
    ScarlettMath::Vec3 rotation      { 0.0f };
    ScarlettMath::Vec3 scale         { 1.0f };
};

} // Namespace ScarlettGame.
