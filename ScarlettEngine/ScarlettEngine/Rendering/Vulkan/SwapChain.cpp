#include "ScarlettEnginePch.h"
#include "SwapChain.h"

#include <vkbootstrap/VkBootstrap.h>

#include "Rendering/Vulkan/Device.h"

namespace Scarlett
{

void SwapChain::Init(Device* device, const VkExtent2D extent)
{
    mDevice = device;

    vkb::SwapchainBuilder swapChainBuilder{ device->GetPhysicalDevice(),device->GetDevice(), device->GetSurface()};

    mFormat = VK_FORMAT_B8G8R8A8_UNORM;

    vkb::Swapchain vkbSwapChain = swapChainBuilder
        .set_desired_format(VkSurfaceFormatKHR{ .format = mFormat, .colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR })
        .set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR)
        .set_desired_extent(extent.width, extent.height)
        .add_image_usage_flags(VK_IMAGE_USAGE_TRANSFER_DST_BIT)
        .build()
        .value();

    mExtent = vkbSwapChain.extent;

    mSwapChain      = vkbSwapChain.swapchain;
    mImages         = vkbSwapChain.get_images().value();
    mImageViews     = vkbSwapChain.get_image_views().value();
}

void SwapChain::Destroy()
{
    vkDestroySwapchainKHR(mDevice->GetDevice(), mSwapChain, nullptr);

    for (size_t i = 0; i < mImages.size(); ++i) 
    {
        vkDestroyImage(mDevice->GetDevice(), mImages[i], nullptr);
        vkDestroyImageView(mDevice->GetDevice(), mImageViews[i], nullptr);
    }

    mImages.clear();
    mImageViews.clear();
}

} // Namespace Scarlett.
