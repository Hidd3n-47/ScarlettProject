#pragma once

#include "ScarlettGameCore/String.h"


//todo make this editor only.
namespace  ScarlettGame
{

struct Tag
{
    ScarlettStl::String name; // TODO need to find a change for cross boundary string implementation.
    ScarlEntt::EntityHandle entity;
};

} // Namespace ScarlettGame.
