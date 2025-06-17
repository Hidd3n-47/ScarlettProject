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
        [this](ScarlEntt::Property* p) { p->mPropertyValue = ScarlEntt::TypeReflection::GetStringFromValue(this->name); return "foo"; },
        [this](const std::string_view& stringValue) { ScarlEntt::TypeReflection::SetValueFromString(this->name, stringValue); } 
    };
};

} // Namespace Scarlett::Component.
