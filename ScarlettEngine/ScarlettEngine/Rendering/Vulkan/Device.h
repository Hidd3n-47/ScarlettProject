#pragma once

#include <vkbootstrap/VkBootstrap.h>

namespace Scarlett
{

class Window;

class Device
{
public:
    void Init(Window* windowRef);
    void Destroy();

    [[nodiscard]] inline VkSurfaceKHR       GetSurface()            const { return mSurface; }
    [[nodiscard]] inline VkPhysicalDevice   GetPhysicalDevice()     const { return mPhysicalDevice; }
    [[nodiscard]] inline VkDevice           GetDevice()             const { return mDevice; }

    const uint32 FRAMES = 2;
private:
    Window* mWindowRef;

    VkInstance                  mInstance;
    VkDebugUtilsMessengerEXT    mDebugMessenger;

    VkSurfaceKHR                mSurface;

    VkPhysicalDevice            mPhysicalDevice;
    VkDevice                    mDevice;

    uint32                      mGraphicsQueueFamily;
    VkQueue                     mGraphicsQueue;

    void InitVulkan();
};

} // Namespace Scarlett.