#pragma once

#include "ScarlEntt/Debug.h"

namespace ScarlEntt
{
class XmlNode;
} // Namespace XmlNode.

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
        { "start" , ScarlEntt::TypeReflection::Reflect(&start) },
        { "end"   , ScarlEntt::TypeReflection::Reflect(&end) },
        { "color" , ScarlEntt::TypeReflection::Reflect(&color) })

    static Line DeserializeComponent(const ScarlEntt::XmlNode* node);
};

} // Namespace Scarlett::Component.
