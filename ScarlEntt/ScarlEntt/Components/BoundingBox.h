#pragma once

#include <Math/Math.h>

namespace Scarlett::Component
{

//todo make this editor only.
struct BoundingBox
{
    ScarlettMath::Vec3 localMinimum { -0.5f, -0.5f, -0.01f };
    ScarlettMath::Vec3 localMaximum {  0.5f,  0.5f,  0.01f };

    inline ScarlettMath::Vec3 GetCenter() const { return (localMinimum + localMaximum) * 0.5f; }

    COMPONENT_SERIALIZATION(
        { "localMinimum", SerializationUtils::ToString(localMinimum) },
        { "localMaximum", SerializationUtils::ToString(localMaximum) })
};

} // Namespace Scarlett::Component.
