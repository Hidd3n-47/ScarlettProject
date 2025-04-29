#pragma once

#include "ScarlEntt/EntityHandle.h"

#include "ScarlEntt/Debug.h"
#include "Serialization/Xml/XmlDocument.h"

namespace ScarlEntt
{
class EntityHandle;
}

//todo make this editor only.
namespace Scarlett::Component
{

struct Tag
{
    Tag() = default;
    Tag(std::string name, const ScarlEntt::EntityHandle& handle)
        : name(std::move(name)), entity(handle)
    { /* Empty. */}

    std::string name;
    ScarlEntt::EntityHandle entity;

    COMPONENT_SERIALIZATION({ "name", ScarlEntt::TypeReflection::Reflect(&name) })

    static Tag DeserializeComponent(const ScarlEntt::XmlNode* node)
    {
        Tag component;
        //todo assert for children size.
        for (const ScarlEntt::XmlNode* childNode : node->GetChildren())
        {
            if (childNode->GetTagName() == "name")
            {
                component.name = ScarlEntt::TypeReflection::GetValueFromTypeString<std::string>(childNode->GetValue());
            }
        }
        //todo need to fix the entity handle somehow.

        return component;
    }
};

} // Namespace Scarlett::Component.
