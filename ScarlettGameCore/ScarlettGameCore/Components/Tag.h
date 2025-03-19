#pragma once

#include "ScarlettGameCore/Src/ScarlettGame.h"

#include "ScarlettGameCore/String.h"

namespace  ScarlettGame
{

struct SCARLETT_GAME_CORE_API Tag
{
    ScarlettStl::String name; // TODO need to find a change for cross boundary string implementation.
    ScarlEntt::EntityHandle entity;
};

} // Namespace ScarlettGame.
