#include "ScarlEnttpch.h"
#include "Components/SquareSprite.h"

#include "RTTI/TypeReflection.h"
#include "ScarlEntt/ComponentManager.h"

namespace Scarlett::Component
{

void SquareSprite::GenerateProperties()
{
    mProperties.clear();

    mProperties["color"] = ScarlEntt::Property { 
        ScarlEntt::PropertyType::VEC4, 
        ScarlEntt::ComponentManager::GetComponentTypeId<SquareSprite>(),
        [this]() { return ScarlEntt::TypeReflection::GetStringFromValue(this->color); },
        [this](const std::string& stringValue) { ScarlEntt::TypeReflection::SetValueFromString(this->color, stringValue); } 
    };
};

} // Namespace Scarlett::Component.
