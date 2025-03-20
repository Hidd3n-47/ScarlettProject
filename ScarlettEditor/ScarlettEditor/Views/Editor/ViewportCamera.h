#pragma once

#include <ScarlettGameCore/Components/Camera.h>

namespace ScarlettGame
{
struct Camera;
}

namespace ScarlettEditor
{

struct ViewportCamera final : public ScarlettGame::Camera { };

} // Namespace ScarlettEditor.
