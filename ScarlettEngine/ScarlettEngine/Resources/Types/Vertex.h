#pragma once

#include <Math/Math.h>

namespace Scarlett::Resource
{

struct Vertex
{
    ScarlettMath::Vec3 position;
    ScarlettMath::Vec3 normal;
    ScarlettMath::Vec2 textureCoord;
};

} // Namespace Scarlett::Resource.