#pragma once

#include <Math/Math.h>

#include <Components/Material.h>

namespace Scarlett {

struct alignas(16) ModelInfo
{
    ScarlettMath::Mat4 view      = ScarlettMath::Mat4{ 1.0f };
    ScarlettMath::Mat4 proj      = ScarlettMath::Mat4{ 1.0f };
    ScarlettMath::Mat4 model     = ScarlettMath::Mat4{ 1.0f };

    ScarlettMath::Vec4 color = ScarlettMath::Vec4{ 1.0f };

    MaterialUploadInfo material;
};

} // Namespace Scarlett.
