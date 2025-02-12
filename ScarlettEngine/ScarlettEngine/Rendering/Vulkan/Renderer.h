#pragma once

#include "Rendering/Vulkan/Device.h"
#include "Rendering/Vulkan/SwapChain.h"

namespace Scarlett
{

class Renderer
{
public:
    static void Init(Window* windowRef);
    static void Destroy();

private:
    static Device      mDevice;
    static SwapChain   mSwapChain;
};

} // Namespace Scarlett.