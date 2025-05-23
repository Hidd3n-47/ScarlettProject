#include "ScarlEnttpch.h"
#include "Components/Tag.h"

namespace Scarlett::Component
{

std::unordered_map<std::string, Property> Tag::properties = {
    {
        "name",
            Property{ ScarlEntt::ValueType::STRING,
            [](void* o) { return SerializationUtils::ToString(static_cast<Tag*>(o)->name); },
            [](void* o, const std::string& value) { static_cast<Tag*>(o)->name = ScarlEntt::TypeReflection::GetValueFromTypeString<std::string>(value); } }
    },
};

Tag Tag::DeserializeComponent(const ScarlEntt::XmlNode* node)
{
    Tag component;

    for (const auto& [propertyName, property] : properties)
    {
        for (const ScarlEntt::XmlNode* childNode : node->GetChildren())
        {
            if (childNode->GetTagName() == propertyName)
            {
                property.SetValue(&component, childNode->GetValue());
            }
        }
    }

    return component;
}

std::unordered_map<std::string, ScarlEntt::TypeReflection>* Tag::GetSerializedFunction()
{
     /* Regenerate the map to ensure it's up to date. */
     mTypeReflectionMap.clear();
     for (const auto& [propertyName, property] : properties)
     {
        mTypeReflectionMap[propertyName] = ScarlEntt::TypeReflection{ property.GetType(), property.GetValueAsString(this) };
     }
     
     return &mTypeReflectionMap;
 }

} // Namespace Scarlett::Component.
