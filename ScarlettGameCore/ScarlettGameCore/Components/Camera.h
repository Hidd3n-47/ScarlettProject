#pragma once

#include <Math/Math.h>

#include "ScarlettGameCore/Src/ScarlettGame.h"

namespace ScarlettGame
{

struct SCARLETT_GAME_CORE_API Camera
{
public:
    Camera();

    ScarlettMath::Mat4 viewMatrix{ 1.0f };
    ScarlettMath::Mat4 projectionMatrix{ 1.0f };
};

} // Namespace ScarlettGame.
