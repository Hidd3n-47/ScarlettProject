#pragma once

#include <glm/vec3.hpp>

#include "ScarlettGame/Src/ScarlettGame.h"

namespace Scarlett
{

struct SCARLETT_GAME_API Transform
{
    glm::vec3 translation   { 0.0f };
    glm::vec3 rotation      { 0.0f };
    glm::vec3 scale         { 1.0f };

    void Translate(const glm::vec3 _translation);
};

} // Namespace Scarlett.
