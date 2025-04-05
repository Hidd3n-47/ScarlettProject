#pragma once

#include <Math/Quat.h>

namespace ScarlettGame
{

struct Transform
{
    ScarlettMath::Vec3 translation   { 0.0f };
    ScarlettMath::Quat rotation      = ScarlettMath::Quat::Identity();
    ScarlettMath::Vec3 scale         { 1.0f };
};

} // Namespace ScarlettGame.
