#pragma once

#include <ScarlettUtils/Xml/XmlDocument.h>

namespace ScarlEntt
{

class SerializationUtils
{
public:
    template <typename T>
    [[nodiscard]] inline static T DeserializeComponent(const ScarlettUtils::XmlElement* node)
    {
        T component;

        for (const auto& [propertyName, property] : *component.GetProperties())
        {
            for (const ScarlettUtils::XmlElement& childNode : node->GetChildElements())
            {
                if (childNode.GetTagName() == propertyName)
                {
                    property.SetPropertyValue(childNode.GetValue());
                }
            }
        }

        return component;
    }
};

} // Namespace ScarlEntt.
