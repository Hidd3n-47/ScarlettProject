#include "ScarlEnttpch.h"
#include "Components/BoundingBox.h"

#include "RTTI/TypeReflection.h"
#include "ScarlEntt/ComponentManager.h"

namespace Scarlett::Component
{

void BoundingBox::GenerateProperties()
{
    mProperties.clear();

    mProperties["localMinimum"] = ScarlEntt::Property { 
        ScarlEntt::PropertyType::VEC3, 
        ScarlEntt::ComponentManager::GetComponentTypeId<BoundingBox>(),
        [this]() { return ScarlEntt::TypeReflection::GetStringFromValue(this->localMinimum); },
        [this](const std::string& stringValue) { ScarlEntt::TypeReflection::SetValueFromString(this->localMinimum, stringValue); } 
    };

    mProperties["localMaximum"] = ScarlEntt::Property { 
        ScarlEntt::PropertyType::VEC3, 
        ScarlEntt::ComponentManager::GetComponentTypeId<BoundingBox>(),
        [this]() { return ScarlEntt::TypeReflection::GetStringFromValue(this->localMaximum); },
        [this](const std::string& stringValue) { ScarlEntt::TypeReflection::SetValueFromString(this->localMaximum, stringValue); } 
    };
};

} // Namespace Scarlett::Component.
