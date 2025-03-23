#pragma once

#include <ScarlettGameCore/Components/Camera.h>

namespace ScarlettGame
{
struct Camera;
}

namespace ScarlettEditor
{

struct ViewportCamera final : public ScarlettGame::Camera
{
    float viewportWidth     = 1920.0f;
    float viewportHeight    = 1080.0f;
};

} // Namespace ScarlettEditor.
