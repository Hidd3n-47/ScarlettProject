#pragma once

#include "ScarlEntt/EntityHandle.h"

#include "ScarlEntt/Debug.h"

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

    SCARLETT_COMPONENT(Tag)
};

} // Namespace Scarlett::Component.
