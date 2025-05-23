#include "ScarlEnttpch.h"
#include "Components/Camera.h"

namespace Scarlett::Component
{

std::unordered_map<std::string, Property> Camera::properties = {
    {
        "mForwardVector",
            Property{ ScarlEntt::ValueType::VEC3,
            [](void* o) { return SerializationUtils::ToString(static_cast<Camera*>(o)->mForwardVector); },
            [](void* o, const std::string& value) { static_cast<Camera*>(o)->mForwardVector = ScarlEntt::TypeReflection::GetValueFromTypeString<ScarlettMath::Vec3>(value); } }
    },
    {
        "mRightVector",
            Property{ ScarlEntt::ValueType::VEC3,
            [](void* o) { return SerializationUtils::ToString(static_cast<Camera*>(o)->mRightVector); },
            [](void* o, const std::string& value) { static_cast<Camera*>(o)->mRightVector = ScarlEntt::TypeReflection::GetValueFromTypeString<ScarlettMath::Vec3>(value); } }
    },
    {
        "mUpVector",
            Property{ ScarlEntt::ValueType::VEC3,
            [](void* o) { return SerializationUtils::ToString(static_cast<Camera*>(o)->mUpVector); },
            [](void* o, const std::string& value) { static_cast<Camera*>(o)->mUpVector = ScarlEntt::TypeReflection::GetValueFromTypeString<ScarlettMath::Vec3>(value); } }
    },
    {
        "mAspectRatio",
            Property{ ScarlEntt::ValueType::FLOAT,
            [](void* o) { return SerializationUtils::ToString(static_cast<Camera*>(o)->mAspectRatio); },
            [](void* o, const std::string& value) { static_cast<Camera*>(o)->mAspectRatio = ScarlEntt::TypeReflection::GetValueFromTypeString<float>(value); } }
    },
};

Camera Camera::DeserializeComponent(const ScarlEntt::XmlNode* node)
{
    Camera component;

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

std::unordered_map<std::string, ScarlEntt::TypeReflection>* Camera::GetSerializedFunction()
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
