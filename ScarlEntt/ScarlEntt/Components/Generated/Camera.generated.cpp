#include "ScarlEnttpch.h"
#include "Components/Camera.h"

#include "RTTI/TypeReflection.h"
#include "ScarlEntt/ComponentManager.h"

namespace Scarlett::Component
{

void Camera::GenerateProperties()
{
    mProperties.clear();

    mProperties["mForwardVector"] = ScarlEntt::Property { 
        ScarlEntt::PropertyType::VEC3, 
        ScarlEntt::ComponentManager::GetComponentTypeId<Camera>(),
        [this]() { return ScarlEntt::TypeReflection::GetStringFromValue(this->mForwardVector); },
        [this](const std::string& stringValue) { ScarlEntt::TypeReflection::SetValueFromString(this->mForwardVector, stringValue); } 
    };

    mProperties["mRightVector"] = ScarlEntt::Property { 
        ScarlEntt::PropertyType::VEC3, 
        ScarlEntt::ComponentManager::GetComponentTypeId<Camera>(),
        [this]() { return ScarlEntt::TypeReflection::GetStringFromValue(this->mRightVector); },
        [this](const std::string& stringValue) { ScarlEntt::TypeReflection::SetValueFromString(this->mRightVector, stringValue); } 
    };

    mProperties["mUpVector"] = ScarlEntt::Property { 
        ScarlEntt::PropertyType::VEC3, 
        ScarlEntt::ComponentManager::GetComponentTypeId<Camera>(),
        [this]() { return ScarlEntt::TypeReflection::GetStringFromValue(this->mUpVector); },
        [this](const std::string& stringValue) { ScarlEntt::TypeReflection::SetValueFromString(this->mUpVector, stringValue); } 
    };

    mProperties["mAspectRatio"] = ScarlEntt::Property { 
        ScarlEntt::PropertyType::FLOAT, 
        ScarlEntt::ComponentManager::GetComponentTypeId<Camera>(),
        [this]() { return ScarlEntt::TypeReflection::GetStringFromValue(this->mAspectRatio); },
        [this](const std::string& stringValue) { ScarlEntt::TypeReflection::SetValueFromString(this->mAspectRatio, stringValue); } 
    };
};

} // Namespace Scarlett::Component.
