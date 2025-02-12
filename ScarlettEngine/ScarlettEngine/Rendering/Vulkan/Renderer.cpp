#include "ScarlettEnginePch.h"
#include "Renderer.h"

#include "Core/Window/Window.h"

namespace Scarlett
{

Device Renderer::mDevice = Device();
SwapChain Renderer::mSwapChain = SwapChain();

void Renderer::Init(Window* windowRef)
{
    mDevice.Init(windowRef);
    mSwapChain.Init(&mDevice, { windowRef->GetWidth(), windowRef->GetHeight() });
}

void Renderer::Destroy()
{
    mSwapChain.Destroy();
    mDevice.Destroy();
}

} // Namespace Scarlett.