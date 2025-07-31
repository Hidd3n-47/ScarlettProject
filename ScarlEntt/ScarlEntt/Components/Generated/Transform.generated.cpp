#include "ScarlEnttpch.h"
#include "Components/Transform.h"

#include "RTTI/TypeReflection.h"
#include "ScarlEntt/ComponentManager.h"

namespace Scarlett::Component
{

#ifdef DEV_CONFIGURATION

void Transform::GenerateProperties()
{
    mProperties.clear();

    mProperties["translation"] = ScarlEntt::Property {
        ScarlEntt::PropertyType::VEC3,
        ScarlEntt::ComponentManager::GetComponentTypeId<Transform>(),
        [this]() { return ScarlEntt::TypeReflection::GetStringFromValue(this->translation); },
        [this](const std::string_view& stringValue) { ScarlEntt::TypeReflection::SetValueFromString(this->translation, stringValue); }
    };

    mProperties["rotation"] = ScarlEntt::Property {
        ScarlEntt::PropertyType::QUAT,
        ScarlEntt::ComponentManager::GetComponentTypeId<Transform>(),
        [this]() { return ScarlEntt::TypeReflection::GetStringFromValue(this->rotation); },
        [this](const std::string_view& stringValue) { ScarlEntt::TypeReflection::SetValueFromString(this->rotation, stringValue); }
    };

    mProperties["scale"] = ScarlEntt::Property {
        ScarlEntt::PropertyType::VEC3,
        ScarlEntt::ComponentManager::GetComponentTypeId<Transform>(),
        [this]() { return ScarlEntt::TypeReflection::GetStringFromValue(this->scale); },
        [this](const std::string_view& stringValue) { ScarlEntt::TypeReflection::SetValueFromString(this->scale, stringValue); }
    };
};

#endif // DEV_CONFIGURATION.

} // Namespace Scarlett::Component.
