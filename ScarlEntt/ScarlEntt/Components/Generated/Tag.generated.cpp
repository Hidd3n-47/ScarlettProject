#include "ScarlEnttpch.h"
#include "Components/Tag.h"

#include "RTTI/TypeReflection.h"
#include "ScarlEntt/ComponentManager.h"

namespace Scarlett::Component
{

#ifdef DEV_CONFIGURATION

void Tag::GenerateProperties()
{
    mProperties.clear();

    mProperties["name"] = ScarlEntt::Property {
        ScarlEntt::PropertyType::STRING,
        ScarlEntt::ComponentManager::GetComponentTypeId<Tag>(),
        [this]() { return ScarlEntt::TypeReflection::GetStringFromValue(this->name); },
        [this](const std::string_view& stringValue) { ScarlEntt::TypeReflection::SetValueFromString(this->name, stringValue); }
    };
};

#endif // DEV_CONFIGURATION.

} // Namespace Scarlett::Component.
