#include "ScarlEnttpch.h"
#include "Components/Line.h"

#include "RTTI/TypeReflection.h"
#include "ScarlEntt/ComponentManager.h"

namespace Scarlett::Component
{

void Line::GenerateProperties()
{
    mProperties.clear();

    mProperties["start"] = ScarlEntt::Property { 
        ScarlEntt::PropertyType::VEC3, 
        ScarlEntt::ComponentManager::GetComponentTypeId<Line>(),
        [this]() { return ScarlEntt::TypeReflection::GetStringFromValue(this->start); },
        [this](const std::string& stringValue) { ScarlEntt::TypeReflection::SetValueFromString(this->start, stringValue); } 
    };

    mProperties["end"] = ScarlEntt::Property { 
        ScarlEntt::PropertyType::VEC3, 
        ScarlEntt::ComponentManager::GetComponentTypeId<Line>(),
        [this]() { return ScarlEntt::TypeReflection::GetStringFromValue(this->end); },
        [this](const std::string& stringValue) { ScarlEntt::TypeReflection::SetValueFromString(this->end, stringValue); } 
    };

    mProperties["color"] = ScarlEntt::Property { 
        ScarlEntt::PropertyType::VEC4, 
        ScarlEntt::ComponentManager::GetComponentTypeId<Line>(),
        [this]() { return ScarlEntt::TypeReflection::GetStringFromValue(this->color); },
        [this](const std::string& stringValue) { ScarlEntt::TypeReflection::SetValueFromString(this->color, stringValue); } 
    };
};

} // Namespace Scarlett::Component.
