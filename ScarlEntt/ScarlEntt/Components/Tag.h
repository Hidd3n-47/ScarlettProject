#pragma once

#include "String.h"
#include "ScarlEntt/EntityHandle.h"

//todo make this editor only.
namespace  Scarlett::Component
{

struct Tag
{
    ScarlettStl::String name; // TODO need to find a change for cross boundary string implementation.
    ScarlEntt::EntityHandle entity;
};

} // Namespace Scarlett::Component.
