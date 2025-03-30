#pragma once

#include <Math/Math.h>

//todo make this editor only.
namespace ScarlettGame
{

struct BoundingBox
{
    ScarlettMath::Vec3 localMinimum { -0.5f, -0.5f, -0.01f };
    ScarlettMath::Vec3 localMaximum {  0.5f,  0.5f,  0.01f };

    inline ScarlettMath::Vec3 GetCenter() const { return (localMinimum + localMaximum) * 0.5f; }
};

} // Namespace ScarlettGameCore.
