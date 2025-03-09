#pragma once

#include "ScarlettGame/Src/ScarlettGame.h"

namespace ScarlEntt
{
    class EntityHandle;
}

namespace  Scarlett
{

struct SCARLETT_GAME_API Tag
{
    const char* name; // TODO need to find a change for cross boundary string implementation.
    ScarlEntt::EntityHandle entity;
};

} // Namespace Scarlett.
