#pragma once

#include <glm/vec3.hpp>

namespace ScarlettGame
{

struct Transform
{
    glm::vec3 translation   { 0.0f };
    glm::vec3 rotation      { 0.0f };
    glm::vec3 scale         { 1.0f };
};

} // Namespace ScarlettGame.
