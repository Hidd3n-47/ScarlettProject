#include "ScarlEnttpch.h"
#include "Components/Transform.h"

#include "RTTI/TypeReflection.h"
#include "ScarlEntt/ComponentManager.h"

namespace Scarlett::Component
{

void Transform::GenerateProperties()
{
    mProperties.clear();

    mProperties["translation"] = ScarlEntt::Property { 
        ScarlEntt::PropertyType::VEC3, 
        ScarlEntt::ComponentManager::GetComponentTypeId<Transform>(),
        [this](ScarlEntt::Property* p) { p->mPropertyValue = ScarlEntt::TypeReflection::GetStringFromValue(this->translation); return "foo"; },
        [this](const std::string_view& stringValue) { ScarlEntt::TypeReflection::SetValueFromString(this->translation, stringValue); } 
    };

    mProperties["rotation"] = ScarlEntt::Property { 
        ScarlEntt::PropertyType::QUAT, 
        ScarlEntt::ComponentManager::GetComponentTypeId<Transform>(),
        [this](ScarlEntt::Property* p) { p->mPropertyValue = ScarlEntt::TypeReflection::GetStringFromValue(this->rotation); return "foo"; },
        [this](const std::string_view& stringValue) { ScarlEntt::TypeReflection::SetValueFromString(this->rotation, stringValue); } 
    };

    mProperties["scale"] = ScarlEntt::Property { 
        ScarlEntt::PropertyType::VEC3, 
        ScarlEntt::ComponentManager::GetComponentTypeId<Transform>(),
        [this](ScarlEntt::Property* p) { p->mPropertyValue = ScarlEntt::TypeReflection::GetStringFromValue(this->scale); return "foo"; },
        [this](const std::string_view& stringValue) { ScarlEntt::TypeReflection::SetValueFromString(this->scale, stringValue); } 
    };
};

} // Namespace Scarlett::Component.
