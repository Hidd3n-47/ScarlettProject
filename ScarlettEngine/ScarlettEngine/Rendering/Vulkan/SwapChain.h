#pragma once

#include <vulkan/vulkan.h>

namespace Scarlett
{

class Device;

class SwapChain
{
public:
    void Init(Device* device, const VkExtent2D extent);
    void Destroy();

private:
    Device*                 mDevice;

    VkSwapchainKHR          mSwapChain;
    VkFormat                mFormat;
    VkExtent2D              mExtent;

    vector<VkImage>         mImages;
    vector<VkImageView>     mImageViews;
};

} // Namespace Scarlett.