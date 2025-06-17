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
        [this](ScarlEntt::Property* p) { p->mPropertyValue = ScarlEntt::TypeReflection::GetStringFromValue(this->localMinimum); return "foo"; },
        [this](const std::string_view& stringValue) { ScarlEntt::TypeReflection::SetValueFromString(this->localMinimum, stringValue); } 
    };

    mProperties["localMaximum"] = ScarlEntt::Property { 
        ScarlEntt::PropertyType::VEC3, 
        ScarlEntt::ComponentManager::GetComponentTypeId<BoundingBox>(),
        [this](ScarlEntt::Property* p) { p->mPropertyValue = ScarlEntt::TypeReflection::GetStringFromValue(this->localMaximum); return "foo"; },
        [this](const std::string_view& stringValue) { ScarlEntt::TypeReflection::SetValueFromString(this->localMaximum, stringValue); } 
    };
};

} // Namespace Scarlett::Component.
