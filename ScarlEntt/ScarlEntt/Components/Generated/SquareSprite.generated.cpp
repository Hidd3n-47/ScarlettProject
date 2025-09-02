#include "ScarlEnttpch.h"
#include "Components/SquareSprite.h"

#include "RTTI/TypeReflection.h"
#include "ScarlEntt/ComponentManager.h"

namespace Scarlett::Component
{

#ifdef DEV_CONFIGURATION

void SquareSprite::GenerateProperties()
{
    mProperties.clear();

    mProperties["color"] = ScarlEntt::Property {
        ScarlEntt::PropertyType::VEC4,
        ScarlEntt::ComponentManager::GetComponentTypeId<SquareSprite>(),
        [this]() { return ScarlEntt::TypeReflection::GetStringFromValue(this->color); },
        [this](const std::string_view& stringValue) { ScarlEntt::TypeReflection::SetValueFromString(this->color, stringValue); }
    };

    mProperties["material"] = ScarlEntt::Property {
        ScarlEntt::PropertyType::MATERIAL,
        ScarlEntt::ComponentManager::GetComponentTypeId<SquareSprite>(),
        [this]() { return ScarlEntt::TypeReflection::GetStringFromValue(this->material); },
        [this](const std::string_view& stringValue) { ScarlEntt::TypeReflection::SetValueFromString(this->material, stringValue); }
    };
};

#endif // DEV_CONFIGURATION.

} // Namespace Scarlett::Component.
