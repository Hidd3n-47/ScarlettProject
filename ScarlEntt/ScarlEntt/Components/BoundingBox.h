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
        { "localMinimum", ScarlEntt::TypeReflection::Reflect(&localMinimum) },
        { "localMaximum", ScarlEntt::TypeReflection::Reflect(&localMaximum) })

    static BoundingBox DeserializeComponent(const ScarlEntt::XmlNode* node)
    {
        BoundingBox component;
        //todo assert for children size.
        for (const ScarlEntt::XmlNode* childNode : node->GetChildren())
        {
            if (childNode->GetTagName() == "localMinimum")
            {
                component.localMinimum = ScarlEntt::TypeReflection::GetValueFromTypeString<ScarlettMath::Vec3>(childNode->GetValue());
                continue;
            }
            if (childNode->GetTagName() == "localMaximum")
            {
                component.localMaximum = ScarlEntt::TypeReflection::GetValueFromTypeString<ScarlettMath::Vec3>(childNode->GetValue());
            }
        }

        return component;
    }
};

} // Namespace Scarlett::Component.
