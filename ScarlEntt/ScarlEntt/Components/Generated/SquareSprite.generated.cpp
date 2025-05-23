#include "ScarlEnttpch.h"
#include "Components/SquareSprite.h"

namespace Scarlett::Component
{

std::unordered_map<std::string, Property> SquareSprite::properties = {
    {
        "color",
            Property{ ScarlEntt::ValueType::VEC4,
            [](void* o) { return SerializationUtils::ToString(static_cast<SquareSprite*>(o)->color); },
            [](void* o, const std::string& value) { static_cast<SquareSprite*>(o)->color = ScarlEntt::TypeReflection::GetValueFromTypeString<ScarlettMath::Vec4>(value); } }
    },
};

SquareSprite SquareSprite::DeserializeComponent(const ScarlEntt::XmlNode* node)
{
    SquareSprite component;

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

std::unordered_map<std::string, ScarlEntt::TypeReflection>* SquareSprite::GetSerializedFunction()
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
