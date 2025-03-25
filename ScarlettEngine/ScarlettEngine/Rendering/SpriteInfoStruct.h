#pragma once

#include <Math/Math.h>

namespace Scarlett {

struct SpriteInfoStruct
{
    ScarlettMath::Vec4 color     = ScarlettMath::Vec4{ 1.0f };

    ScarlettMath::Mat4 view      = ScarlettMath::Mat4{1.0f };
    ScarlettMath::Mat4 proj      = ScarlettMath::Mat4{1.0f };
    ScarlettMath::Mat4 model     = ScarlettMath::Mat4{ 1.0f };
};

} // Namespace Scarlett.
