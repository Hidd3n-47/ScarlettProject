#include "ScarlEnttpch.h"
#include "Components/BoundingBox.h"

namespace Scarlett::Component
{

std::unordered_map<std::string, Property> BoundingBox::properties = {
    {
        "localMinimum",
            Property{ ScarlEntt::ValueType::VEC3,
            [](void* o) { return SerializationUtils::ToString(static_cast<BoundingBox*>(o)->localMinimum); },
            [](void* o, const std::string& value) { static_cast<BoundingBox*>(o)->localMinimum = ScarlEntt::TypeReflection::GetValueFromTypeString<ScarlettMath::Vec3>(value); } }
    },
    {
        "localMaximum",
            Property{ ScarlEntt::ValueType::VEC3,
            [](void* o) { return SerializationUtils::ToString(static_cast<BoundingBox*>(o)->localMaximum); },
            [](void* o, const std::string& value) { static_cast<BoundingBox*>(o)->localMaximum = ScarlEntt::TypeReflection::GetValueFromTypeString<ScarlettMath::Vec3>(value); } }
    },
};

BoundingBox BoundingBox::DeserializeComponent(const ScarlEntt::XmlNode* node)
{
    BoundingBox component;

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

std::unordered_map<std::string, ScarlEntt::TypeReflection>* BoundingBox::GetSerializedFunction()
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
