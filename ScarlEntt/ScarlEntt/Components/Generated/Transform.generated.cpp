#include "ScarlEnttpch.h"
#include "Components/Transform.h"

namespace Scarlett::Component
{

std::unordered_map<std::string, Property> Transform::properties = {
    {
        "translation",
            Property{ ScarlEntt::ValueType::VEC3,
            [](void* o) { return SerializationUtils::ToString(static_cast<Transform*>(o)->translation); },
            [](void* o, const std::string& value) { static_cast<Transform*>(o)->translation = ScarlEntt::TypeReflection::GetValueFromTypeString<ScarlettMath::Vec3>(value); } }
    },
    {
        "rotation",
            Property{ ScarlEntt::ValueType::QUAT,
            [](void* o) { return SerializationUtils::ToString(static_cast<Transform*>(o)->rotation); },
            [](void* o, const std::string& value) { static_cast<Transform*>(o)->rotation = ScarlEntt::TypeReflection::GetValueFromTypeString<ScarlettMath::Quat>(value); } }
    },
    {
        "scale",
            Property{ ScarlEntt::ValueType::VEC3,
            [](void* o) { return SerializationUtils::ToString(static_cast<Transform*>(o)->scale); },
            [](void* o, const std::string& value) { static_cast<Transform*>(o)->scale = ScarlEntt::TypeReflection::GetValueFromTypeString<ScarlettMath::Vec3>(value); } }
    },
};

Transform Transform::DeserializeComponent(const ScarlEntt::XmlNode* node)
{
    Transform component;

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

std::unordered_map<std::string, ScarlEntt::TypeReflection>* Transform::GetSerializedFunction()
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
