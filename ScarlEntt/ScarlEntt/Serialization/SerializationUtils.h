#pragma once

#include "Xml/XmlDocument.h"

namespace ScarlEntt
{

class SerializationUtils
{
public:
    template <typename T>
    [[nodiscard]] inline static T DeserializeComponent(const XmlNode* node)
    {
        T component;

        for (const auto& [propertyName, property] : *component.GetProperties())
        {
            for (const XmlNode* childNode : node->GetChildren())
            {
                if (childNode->GetTagName() == propertyName)
                {
                    property.SetPropertyValue(childNode->GetValue());
                }
            }
        }

        return component;
    }
};

} // Namespace ScarlEntt.
