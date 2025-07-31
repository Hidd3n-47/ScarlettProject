#include "ScarlEnttpch.h"
#include "Components/BoundingBox.h"

#include "RTTI/TypeReflection.h"
#include "ScarlEntt/ComponentManager.h"

namespace Scarlett::Component
{

#ifdef DEV_CONFIGURATION

void BoundingBox::GenerateProperties()
{
    mProperties.clear();

    mProperties["localMinimum"] = ScarlEntt::Property {
        ScarlEntt::PropertyType::VEC3,
        ScarlEntt::ComponentManager::GetComponentTypeId<BoundingBox>(),
        [this]() { return ScarlEntt::TypeReflection::GetStringFromValue(this->localMinimum); },
        [this](const std::string_view& stringValue) { ScarlEntt::TypeReflection::SetValueFromString(this->localMinimum, stringValue); }
    };

    mProperties["localMaximum"] = ScarlEntt::Property {
        ScarlEntt::PropertyType::VEC3,
        ScarlEntt::ComponentManager::GetComponentTypeId<BoundingBox>(),
        [this]() { return ScarlEntt::TypeReflection::GetStringFromValue(this->localMaximum); },
        [this](const std::string_view& stringValue) { ScarlEntt::TypeReflection::SetValueFromString(this->localMaximum, stringValue); }
    };
};

#endif // DEV_CONFIGURATION.

} // Namespace Scarlett::Component.
