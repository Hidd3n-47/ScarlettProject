#pragma once

#include <Math/Quat.h>

#include "ScarlEntt/Debug.h"
#include "Serialization/Xml/XmlDocument.h"

namespace Scarlett::Component
{

struct Transform
{
    ScarlettMath::Vec3 translation   { 0.0f };
    ScarlettMath::Quat rotation      { ScarlettMath::Quat::Identity() };
    ScarlettMath::Vec3 scale         { 1.0f };

    COMPONENT_SERIALIZATION(
        REFLECT(translation),
        REFLECT(rotation),
        REFLECT(scale))

    static Transform DeserializeComponent(const ScarlEntt::XmlNode* node)
    {
        Transform component;
        //todo assert for children size.
        for (const ScarlEntt::XmlNode* childNode : node->GetChildren())
        {
            if (childNode->GetTagName() == "translation")
            {
                component.translation = ScarlEntt::TypeReflection::GetValueFromTypeString<ScarlettMath::Vec3>(childNode->GetValue());
                continue;
            }
            if (childNode->GetTagName() == "rotation")
            {
                component.rotation = ScarlEntt::TypeReflection::GetValueFromTypeString<ScarlettMath::Quat>(childNode->GetValue());
                continue;
            }
            if (childNode->GetTagName() == "scale")
            {
                component.scale = ScarlEntt::TypeReflection::GetValueFromTypeString<ScarlettMath::Vec3>(childNode->GetValue());
            }
        }

        return component;
    }
};

} // Namespace Scarlett::Component.
