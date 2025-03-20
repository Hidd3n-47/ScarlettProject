#pragma once

#include <glm/vec3.hpp>

#include "ScarlettGameCore/Src/ScarlettGame.h"

namespace ScarlettGame
{

struct SCARLETT_GAME_CORE_API Transform
{
    glm::vec3 translation   { 0.0f };
    glm::vec3 rotation      { 0.0f };
    glm::vec3 scale         { 1.0f };
};

} // Namespace ScarlettGame.
