#pragma once

#include <glm/glm.hpp>

namespace Scarlett
{

struct Transform
{
    glm::vec3 translation   { 0.0f };
    glm::vec3 rotation      { 0.0f };
    glm::vec3 scale         { 1.0f };
};

} // Scarlett.

