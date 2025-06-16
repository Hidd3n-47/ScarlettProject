#include "ScarlEnttpch.h"
#include "Components/Tag.h"

#include "RTTI/TypeReflection.h"
#include "ScarlEntt/ComponentManager.h"

namespace Scarlett::Component
{

void Tag::GenerateProperties()
{
    mProperties.clear();

    mProperties["name"] = ScarlEntt::Property { 
        ScarlEntt::PropertyType::STRING, 
        ScarlEntt::ComponentManager::GetComponentTypeId<Tag>(),
        [this]() { return ScarlEntt::TypeReflection::GetStringFromValue(this->name); },
        [this](const std::string& stringValue) { ScarlEntt::TypeReflection::SetValueFromString(this->name, stringValue); } 
    };
};

} // Namespace Scarlett::Component.
