#include "ScarlEnttpch.h"
#include "Components/Line.h"

namespace Scarlett::Component
{

std::unordered_map<std::string, Property> Line::properties = {
    {
        "start",
            Property{ ScarlEntt::ValueType::VEC3,
            [](void* o) { return SerializationUtils::ToString(static_cast<Line*>(o)->start); },
            [](void* o, const std::string& value) { static_cast<Line*>(o)->start = ScarlEntt::TypeReflection::GetValueFromTypeString<ScarlettMath::Vec3>(value); } }
    },
    {
        "end",
            Property{ ScarlEntt::ValueType::VEC3,
            [](void* o) { return SerializationUtils::ToString(static_cast<Line*>(o)->end); },
            [](void* o, const std::string& value) { static_cast<Line*>(o)->end = ScarlEntt::TypeReflection::GetValueFromTypeString<ScarlettMath::Vec3>(value); } }
    },
    {
        "color",
            Property{ ScarlEntt::ValueType::VEC4,
            [](void* o) { return SerializationUtils::ToString(static_cast<Line*>(o)->color); },
            [](void* o, const std::string& value) { static_cast<Line*>(o)->color = ScarlEntt::TypeReflection::GetValueFromTypeString<ScarlettMath::Vec4>(value); } }
    },
};

Line Line::DeserializeComponent(const ScarlEntt::XmlNode* node)
{
    Line component;

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

std::unordered_map<std::string, ScarlEntt::TypeReflection>* Line::GetSerializedFunction()
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
