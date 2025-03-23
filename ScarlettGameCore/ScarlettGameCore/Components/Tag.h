#pragma once

#include "ScarlettGameCore/String.h"

namespace  ScarlettGame
{

struct Tag
{
    ScarlettStl::String name; // TODO need to find a change for cross boundary string implementation.
    ScarlEntt::EntityHandle entity;
};

} // Namespace ScarlettGame.
