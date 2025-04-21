#pragma once

#include <Math/Math.h>

namespace Scarlett::Component
{

//todo make this editor only.
struct Line
{
    Line() = default;
    Line(const ScarlettMath::Vec3 start, const ScarlettMath::Vec3 end)
        : start(start), end(end), color({ 1.0f, 1.0f, 1.0f, 1.0f })
    { /* Empty. */ }

    Line(const ScarlettMath::Vec3 start, const ScarlettMath::Vec3 end, const ScarlettMath::Vec4 color)
        : start(start), end(end), color(color)
    { /* Empty. */ }

    ScarlettMath::Vec3 start;
    ScarlettMath::Vec3 end;

    ScarlettMath::Vec4 color;

    COMPONENT_SERIALIZATION(
        { "start", "1,1,1" },
        { "end", "1,1,1" },
        { "color", "1,1,1,1" })
};

} // Namespace Scarlett::Component.
