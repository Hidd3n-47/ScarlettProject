#pragma once

#include <glm/glm.hpp>

#include "ScarlettGameCore/Src/ScarlettGame.h"

namespace ScarlettGame
{

struct SCARLETT_GAME_CORE_API Camera
{
public:
    Camera();

    glm::mat4 viewMatrix{ 1.0f };
    glm::mat4 projectionMatrix{ 1.0f };
};

} // Namespace ScarlettGame.
