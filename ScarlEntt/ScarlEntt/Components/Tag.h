#pragma once

#include "ScarlEntt/EntityHandle.h"

//todo make this editor only.
namespace Scarlett::Component
{

struct Tag
{
    Tag(std::string name, const ScarlEntt::EntityHandle& handle)
        : name(std::move(name)), entity(handle)
    { /* Empty. */}

    std::string name;
    ScarlEntt::EntityHandle entity;

    COMPONENT_SERIALIZATION({ "name", name })
};

} // Namespace Scarlett::Component.
