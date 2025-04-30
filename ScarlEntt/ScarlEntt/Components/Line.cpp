#include "ScarlEnttpch.h"
#include "Line.h"

#include <Math/Math.h>

#include "Serialization/Xml/XmlDocument.h"

namespace Scarlett::Component
{

Line Line::DeserializeComponent(const ScarlEntt::XmlNode* node)
{
    Line component;
    //todo assert for children size.
    for (const ScarlEntt::XmlNode* childNode : node->GetChildren())
    {
        if (childNode->GetTagName() == "start")
        {
            component.start = ScarlEntt::TypeReflection::GetValueFromTypeString<ScarlettMath::Vec3>(childNode->GetValue());
            continue;
        }
        if (childNode->GetTagName() == "end")
        {
            component.end = ScarlEntt::TypeReflection::GetValueFromTypeString<ScarlettMath::Vec3>(childNode->GetValue());
            continue;
        }
        if (childNode->GetTagName() == "color")
        {
            component.color = ScarlEntt::TypeReflection::GetValueFromTypeString<ScarlettMath::Vec4>(childNode->GetValue());
        }
    }

    return component;
}

} // Namespace Scarlett::Component.
