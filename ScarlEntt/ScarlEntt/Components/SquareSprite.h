#pragma once

#include <Math/Math.h>

#include "ScarlEntt/Debug.h"
#include "Serialization/Xml/XmlDocument.h"

namespace Scarlett::Component
{

    struct SquareSprite
    {
        ScarlettMath::Vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

        COMPONENT_SERIALIZATION(REFLECT(color))

        static SquareSprite DeserializeComponent(const ScarlEntt::XmlNode* node)
        {
            SquareSprite component;
            //todo assert for children size.
            for (const ScarlEntt::XmlNode* childNode : node->GetChildren())
            {
                if (childNode->GetTagName() == "color")
                {
                    component.color = ScarlEntt::TypeReflection::GetValueFromTypeString<ScarlettMath::Vec4>(childNode->GetValue());
                }
            }

            return component;
        }
    };

} // Namespace Scarlett::Component.
