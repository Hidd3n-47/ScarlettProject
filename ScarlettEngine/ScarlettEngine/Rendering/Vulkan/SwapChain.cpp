#include "ScarlettEnginepch.h"
#include "SwapChain.h"

#include "Device.h"
#include "Rendering/Vulkan/VulkanUtils.h"

namespace Scarlett
{

SwapChain::SwapChain(Device* device, const VkExtent2D extent, const SwapChain* previousSwapChain)
    : mDevice{ device }
    , mExtent{ extent }
{
    Init(previousSwapChain);

    delete previousSwapChain;
}

SwapChain::~SwapChain()
{
    Destroy();
}

VkFormat SwapChain::FindDepthFormat() const
{
    return mDevice->FindSupportedFormat({ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT }, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}

VkResult SwapChain::AcquireNextImage(uint32* imageIndex) const
{
    vkWaitForFences(mDevice->GetDevice(), 1, &mInFlightFences[mCurrentFrame], VK_TRUE, std::numeric_limits<uint64_t>::max());

    return vkAcquireNextImageKHR(mDevice->GetDevice(), mSwapChain, std::numeric_limits<uint64_t>::max(), mImageAvailableSemaphores[mCurrentFrame], VK_NULL_HANDLE, imageIndex);
}

VkResult SwapChain::SubmitCommandBuffers(const VkCommandBuffer* buffer, const uint32* imageIndex)
{
    if (mImagesInFlight[*imageIndex] != VK_NULL_HANDLE)
    {
        vkWaitForFences(mDevice->GetDevice(), 1, &mImagesInFlight[*imageIndex], VK_TRUE, UINT64_MAX);
    }
    mImagesInFlight[*imageIndex] = mInFlightFences[mCurrentFrame];

    VkSemaphore             waitSemaphores[]    { mImageAvailableSemaphores[mCurrentFrame] };
    VkPipelineStageFlags    waitStages[]        { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    VkSemaphore             signalSemaphores[]  { mRenderFinishedSemaphores[mCurrentFrame] };

    const VkSubmitInfo submitInfo
    {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .waitSemaphoreCount     = 1,
        .pWaitSemaphores        = waitSemaphores,
        .pWaitDstStageMask      = waitStages,
        .commandBufferCount     = 1,
        .pCommandBuffers        = buffer,
        .signalSemaphoreCount   = 1,
        .pSignalSemaphores      = signalSemaphores,
    };

    vkResetFences(mDevice->GetDevice(), 1, &mInFlightFences[mCurrentFrame]);
    VK_CHECK(vkQueueSubmit(mDevice->GetGraphicsQueue(), 1, &submitInfo, mInFlightFences[mCurrentFrame]), "Failed to submit draw command buffer.");

    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    const VkSwapchainKHR swapChains[] = { mSwapChain };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = imageIndex;

    const auto result = vkQueuePresentKHR(mDevice->GetPresentQueue(), &presentInfo);

    mCurrentFrame = (mCurrentFrame + 1) % MAX_IMAGES_PER_FRAME;

    return result;
}

void SwapChain::Init(const SwapChain* previousSwapChain)
{
    CreateSwapChain(previousSwapChain);
    CreateImageViews();
    CreateSwapChainRenderPass();
    CreateDepthResources();
    CreateSwapChainFrameBuffers();
    CreateSyncObjects();
}

void SwapChain::Destroy()
{
#ifdef SCARLETT_EDITOR_ENABLED
    for(size_t i = 0; i < mViewportImage.size(); ++i)
    {
        vkDestroyImage(mDevice->GetDevice(), mViewportImage[i], nullptr);
        vkDestroyImageView(mDevice->GetDevice(), mViewportImageView[i], nullptr);
        vkFreeMemory(mDevice->GetDevice(), mViewportImageMemory[i], nullptr);
    }

    for (const VkFramebuffer frameBuffer : mEditorSwapChainFrameBuffers)
    {
        vkDestroyFramebuffer(mDevice->GetDevice(), frameBuffer, nullptr);
    }

    vkDestroyRenderPass(mDevice->GetDevice(), mEditorRenderPass, nullptr);
#endif // SCARLETT_EDITOR_ENABLED.

    for (const VkImageView imageView : mSwapChainImageViews)
    {
        vkDestroyImageView(mDevice->GetDevice(), imageView, nullptr);
    }
    mSwapChainImageViews.clear();

    if (mSwapChain != nullptr)
    {
        vkDestroySwapchainKHR(mDevice->GetDevice(), mSwapChain, nullptr);
    }

    for (size_t i = 0; i < mDepthImages.size(); ++i)
    {
        vkDestroyImageView(mDevice->GetDevice(), mDepthImageViews[i], nullptr);
        vkDestroyImage(mDevice->GetDevice(), mDepthImages[i], nullptr);
        vkFreeMemory(mDevice->GetDevice(), mDepthImageMemory[i], nullptr);
    }

    for (const VkFramebuffer frameBuffer : mSwapChainFrameBuffers)
    {
        vkDestroyFramebuffer(mDevice->GetDevice(), frameBuffer, nullptr);
    }

    vkDestroyRenderPass(mDevice->GetDevice(), mRenderPass, nullptr);

    for (int i = 0; i < MAX_IMAGES_PER_FRAME; ++i)
    {
        vkDestroySemaphore(mDevice->GetDevice(), mRenderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(mDevice->GetDevice(), mImageAvailableSemaphores[i], nullptr);
        vkDestroyFence(mDevice->GetDevice(), mInFlightFences[i], nullptr);
    }
}

void SwapChain::CreateSwapChain(const SwapChain* previousSwapChain)
{
    SwapChainDetails swapChainDetails = mDevice->GetSwapChainDetails();

    const VkSurfaceFormatKHR  surfaceFormat       = ChooseSwapSurfaceFormat(swapChainDetails.formats);
    const VkPresentModeKHR    presentationMode    = ChooseSwapPresentMode(swapChainDetails.presentationModes);
    const VkExtent2D          extents             = ChooseSwapChainExtent(swapChainDetails.surfaceCapabilities);

    uint32 imageCount = swapChainDetails.surfaceCapabilities.minImageCount + 1;
    imageCount = swapChainDetails.surfaceCapabilities.maxImageCount != 0 && imageCount < swapChainDetails.surfaceCapabilities.maxImageCount ? swapChainDetails.surfaceCapabilities.minImageCount : imageCount;

    VkSwapchainCreateInfoKHR swapChainCreateInfo
    {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface            = mDevice->GetSurface(),
        .minImageCount      = imageCount,
        .imageFormat        = surfaceFormat.format,
        .imageColorSpace    = surfaceFormat.colorSpace,
        .imageExtent        = extents,
        .imageArrayLayers   = 1,
        .imageUsage         = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .preTransform       = swapChainDetails.surfaceCapabilities.currentTransform,
        .compositeAlpha     = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode        = presentationMode,
        .clipped            = VK_TRUE,
        .oldSwapchain       = previousSwapChain ? previousSwapChain->GetSwapChain() : VK_NULL_HANDLE
    };

    const QueueFamilyIndices indices = mDevice->GetQueueFamilyIndices();
    const uint32 queueFamilyIndices[] { static_cast<uint32>(indices.graphicsFamily), static_cast<uint32>(indices.presentationFamily) };

    if (indices.graphicsFamily != indices.presentationFamily)
    {
        swapChainCreateInfo.imageSharingMode        = VK_SHARING_MODE_CONCURRENT;
        swapChainCreateInfo.queueFamilyIndexCount   = 2;
        swapChainCreateInfo.pQueueFamilyIndices     = queueFamilyIndices;
    }
    else
    {
        swapChainCreateInfo.imageSharingMode        = VK_SHARING_MODE_EXCLUSIVE;
        swapChainCreateInfo.queueFamilyIndexCount   = 0;
        swapChainCreateInfo.pQueueFamilyIndices     = nullptr;
    }

    VK_CHECK(vkCreateSwapchainKHR(mDevice->GetDevice(), &swapChainCreateInfo, nullptr, &mSwapChain), "Failed to create Vulkan Swap Chain.");

    vkGetSwapchainImagesKHR(mDevice->GetDevice(), mSwapChain, &imageCount, nullptr);
    mSwapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(mDevice->GetDevice(), mSwapChain, &imageCount, mSwapChainImages.data());

    mSwapChainImageFormat = surfaceFormat.format;
    mExtent = extents;
}

void SwapChain::CreateImageViews()
{
    mSwapChainImageViews.resize(mSwapChainImages.size());
    for (size_t i = 0; i < mSwapChainImages.size(); ++i)
    {
        constexpr VkImageSubresourceRange subresourceRange
        {
                .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
                .baseMipLevel   = 0,
                .levelCount     = 1,
                .baseArrayLayer = 0,
                .layerCount     = 1
        };

        const VkImageViewCreateInfo viewInfo
        {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .image                              = mSwapChainImages[i],
            .viewType                           = VK_IMAGE_VIEW_TYPE_2D,
            .format                             = mSwapChainImageFormat,
            .subresourceRange                   = subresourceRange
        };

        VK_CHECK(vkCreateImageView(mDevice->GetDevice(), &viewInfo, nullptr, &mSwapChainImageViews[i]), "Failed to create Vulkan Texture Image View.");
    }

#ifdef SCARLETT_EDITOR_ENABLED
    mViewportImage.resize(GetImageCount());
    mViewportImageView.resize(GetImageCount());
    mViewportImageMemory.resize(GetImageCount());

    for (size_t i = 0; i < GetImageCount(); ++i)
    {
        VkImageCreateInfo imageCreateInfo = {};
        imageCreateInfo.sType           = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageCreateInfo.imageType       = VK_IMAGE_TYPE_2D;
        imageCreateInfo.format          = mSwapChainImageFormat;
        imageCreateInfo.extent.width    = mExtent.width;
        imageCreateInfo.extent.height   = mExtent.height;
        imageCreateInfo.extent.depth    = 1;
        imageCreateInfo.mipLevels       = 1;
        imageCreateInfo.arrayLayers     = 1;
        imageCreateInfo.samples         = VK_SAMPLE_COUNT_1_BIT;
        imageCreateInfo.tiling          = VK_IMAGE_TILING_OPTIMAL;
        imageCreateInfo.usage           = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
        imageCreateInfo.sharingMode     = VK_SHARING_MODE_EXCLUSIVE;
        imageCreateInfo.initialLayout   = VK_IMAGE_LAYOUT_UNDEFINED;

        vkCreateImage(mDevice->GetDevice(), &imageCreateInfo, nullptr, &mViewportImage[i]);

        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements(mDevice->GetDevice(), mViewportImage[i], &memRequirements);

        VkMemoryAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize    = memRequirements.size;
        allocInfo.memoryTypeIndex   = mDevice->FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        vkAllocateMemory(mDevice->GetDevice(), &allocInfo, nullptr, &mViewportImageMemory[i]);

        vkBindImageMemory(mDevice->GetDevice(), mViewportImage[i], mViewportImageMemory[i], 0);

        VkImageViewCreateInfo imageViewCreateInfo = {};
        imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imageViewCreateInfo.image                               = mViewportImage[i];
        imageViewCreateInfo.viewType                            = VK_IMAGE_VIEW_TYPE_2D;
        imageViewCreateInfo.format                              = mSwapChainImageFormat;
        imageViewCreateInfo.subresourceRange.aspectMask         = VK_IMAGE_ASPECT_COLOR_BIT;
        imageViewCreateInfo.subresourceRange.baseMipLevel       = 0;
        imageViewCreateInfo.subresourceRange.levelCount         = 1;
        imageViewCreateInfo.subresourceRange.baseArrayLayer     = 0;
        imageViewCreateInfo.subresourceRange.layerCount         = 1;

        vkCreateImageView(mDevice->GetDevice(), &imageViewCreateInfo, nullptr, &mViewportImageView[i]);
    }
#endif // SCARLETT_EDITOR_ENABLED.
}

void SwapChain::CreateSwapChainRenderPass()
{
#ifdef SCARLETT_EDITOR_ENABLED
    CreateRenderPass(true, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, &mRenderPass);
    CreateRenderPass(false, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, &mEditorRenderPass);
#else
    CreateRenderPass(true, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, &mRenderPass);
#endif // SCARLETT_EDITOR_ENABLED.
}

void SwapChain::CreateDepthResources()
{
    const VkFormat      depthFormat      = FindDepthFormat();
    const VkExtent2D    swapChainExtent  = GetSwapChainExtent();

    mDepthImages.resize(GetImageCount());
    mDepthImageMemory.resize(GetImageCount());
    mDepthImageViews.resize(GetImageCount());

    for (size_t i = 0; i < mDepthImages.size(); ++i)
    {
        VkImageCreateInfo imageInfo
        {
            .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
            .flags              = 0,
            .imageType          = VK_IMAGE_TYPE_2D,
            .format             = depthFormat,
            .extent             = { swapChainExtent.width, swapChainExtent.height, 1 },
            .mipLevels          = 1,
            .arrayLayers        = 1,
            .samples            = VK_SAMPLE_COUNT_1_BIT,
            .tiling             = VK_IMAGE_TILING_OPTIMAL,
            .usage              = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
            .sharingMode        = VK_SHARING_MODE_EXCLUSIVE,
            .initialLayout      = VK_IMAGE_LAYOUT_UNDEFINED,
        };

        mDevice->CreateImageWithInfo(imageInfo, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, mDepthImages[i], mDepthImageMemory[i]);

        constexpr VkImageSubresourceRange subResourceRange
        {
            .aspectMask     = VK_IMAGE_ASPECT_DEPTH_BIT,
            .baseMipLevel   = 0,
            .levelCount     = 1,
            .baseArrayLayer = 0,
            .layerCount     = 1
        };

        const VkImageViewCreateInfo viewInfo
        {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .image              = mDepthImages[i],
            .viewType           = VK_IMAGE_VIEW_TYPE_2D,
            .format             = depthFormat,
            .subresourceRange   = subResourceRange

        };

        VK_CHECK(vkCreateImageView(mDevice->GetDevice(), &viewInfo, nullptr, &mDepthImageViews[i]), "Failed to create Vulkan Image Texture View.");
    }
}

void SwapChain::CreateSwapChainFrameBuffers()
{
    const size_t IMAGE_COUNT = GetImageCount();

    mSwapChainFrameBuffers.resize(IMAGE_COUNT);
    for (size_t i = 0; i < IMAGE_COUNT; ++i)
    {
        VkImageView colorImageViewAttachment = mSwapChainImageViews[i];

#ifdef SCARLETT_EDITOR_ENABLED
        colorImageViewAttachment = mViewportImageView[i];
#endif // SCARLETT_EDITOR_ENABLED.

        VkImageView attachments[2] = { colorImageViewAttachment, mDepthImageViews[i] };
        CreateFrameBuffer(mRenderPass, attachments, std::size(attachments), &mSwapChainFrameBuffers[i]);
    }

#ifdef SCARLETT_EDITOR_ENABLED
    mEditorSwapChainFrameBuffers.resize(IMAGE_COUNT);
    for (size_t i = 0; i < IMAGE_COUNT; ++i)
    {
        VkImageView attachments[] = { mSwapChainImageViews[i] };
        CreateFrameBuffer(mEditorRenderPass, attachments, std::size(attachments), &mEditorSwapChainFrameBuffers[i]);
    }
#endif // SCARLETT_EDITOR_ENABLED.
}

void SwapChain::CreateSyncObjects()
{
    mImageAvailableSemaphores.resize(MAX_IMAGES_PER_FRAME);
    mRenderFinishedSemaphores.resize(MAX_IMAGES_PER_FRAME);
    mInFlightFences.resize(MAX_IMAGES_PER_FRAME);
    mImagesInFlight.resize(MAX_IMAGES_PER_FRAME, VK_NULL_HANDLE);

    constexpr VkSemaphoreCreateInfo semaphoreInfo
    {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO
    };

    constexpr VkFenceCreateInfo fenceInfo
    {
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .flags = VK_FENCE_CREATE_SIGNALED_BIT,
    };

    for (int i = 0; i < MAX_IMAGES_PER_FRAME; ++i)
    {
        VK_CHECK(vkCreateSemaphore(mDevice->GetDevice(), &semaphoreInfo, nullptr, &mImageAvailableSemaphores[i]), "Failed to create Vulkan Semaphore.");
        VK_CHECK(vkCreateSemaphore(mDevice->GetDevice(), &semaphoreInfo, nullptr, &mRenderFinishedSemaphores[i]), "Failed to create Vulkan Semaphore.");
        VK_CHECK(vkCreateFence(mDevice->GetDevice(), &fenceInfo, nullptr, &mInFlightFences[i]), "Failed to create Vulkan Fence.");
    }
}

void SwapChain::CreateRenderPass(const bool hasDepthAttachment, const VkImageLayout finalImageLayout, VkRenderPass* renderPass) const
{
    const VkAttachmentDescription depthAttachment
    {
        .format             = FindDepthFormat(),
        .samples            = VK_SAMPLE_COUNT_1_BIT,
        .loadOp             = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp            = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .stencilLoadOp      = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        .stencilStoreOp     = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .initialLayout      = VK_IMAGE_LAYOUT_UNDEFINED,
        .finalLayout        = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
    };

    constexpr VkAttachmentReference depthAttachmentRef
    {
        .attachment = 1,
        .layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
    };

    const VkAttachmentDescription colorAttachment
    {
        .format             = mSwapChainImageFormat,
        .samples            = VK_SAMPLE_COUNT_1_BIT,
        .loadOp             = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp            = VK_ATTACHMENT_STORE_OP_STORE,
        .stencilLoadOp      = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        .stencilStoreOp     = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .initialLayout      = VK_IMAGE_LAYOUT_UNDEFINED,
        .finalLayout        = finalImageLayout
    };

    constexpr VkAttachmentReference colorAttachmentRef
    {
        .attachment = 0,
        .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
    };

    const VkSubpassDescription subPass
    {
        .pipelineBindPoint          = VK_PIPELINE_BIND_POINT_GRAPHICS,
        .colorAttachmentCount       = 1,
        .pColorAttachments          = &colorAttachmentRef,
        .pDepthStencilAttachment    = hasDepthAttachment ? &depthAttachmentRef : nullptr
    };

    constexpr VkSubpassDependency dependency
    {
        .srcSubpass     = VK_SUBPASS_EXTERNAL,
        .dstSubpass     = 0,
        .srcStageMask   = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
        .dstStageMask   = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
        .srcAccessMask  = 0,
        .dstAccessMask  = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT
    };

    vector<VkAttachmentDescription> attachments;
    attachments.emplace_back(colorAttachment);

    if(hasDepthAttachment)
    {
        attachments.emplace_back(depthAttachment);
    }

    const VkRenderPassCreateInfo renderPassInfo
    {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
        .attachmentCount    = static_cast<uint32>(attachments.size()),
        .pAttachments       = attachments.data(),
        .subpassCount       = 1,
        .pSubpasses         = &subPass,
        .dependencyCount    = 1,
        .pDependencies      = &dependency,
    };

    VK_CHECK(vkCreateRenderPass(mDevice->GetDevice(), &renderPassInfo, nullptr, renderPass), "Failed to create Vulkan Render Pass.");
}

void SwapChain::CreateFrameBuffer(const VkRenderPass renderPass, const VkImageView* attachments, const uint32 attachmentCount, VkFramebuffer* frameBuffer) const
{
    const auto [width, height] { GetSwapChainExtent() };

    const VkFramebufferCreateInfo frameBufferInfo
    {
        .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
        .renderPass         = renderPass,
        .attachmentCount    = attachmentCount,
        .pAttachments       = attachments,
        .width              = width,
        .height             = height,
        .layers             = 1
    };

    VK_CHECK(vkCreateFramebuffer(mDevice->GetDevice(), &frameBufferInfo, nullptr, frameBuffer), "Failed to create Vulkan Frame Buffer.");
}

VkSurfaceFormatKHR SwapChain::ChooseSwapSurfaceFormat(const vector<VkSurfaceFormatKHR>& availableFormats)
{
    for (const VkSurfaceFormatKHR& availableFormat : availableFormats)
    {
        if (availableFormat.format == VK_FORMAT_R8G8B8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            return availableFormat;
        }
    }

    return availableFormats[0];
}

VkPresentModeKHR SwapChain::ChooseSwapPresentMode(const vector<VkPresentModeKHR>& availablePresentModes)
{
    for (const auto& availablePresentMode : availablePresentModes)
    {
        if(availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            return availablePresentMode;
        }
    }

    SCARLETT_DLOG("Present mode: V-Sync");
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D SwapChain::ChooseSwapChainExtent(const VkSurfaceCapabilitiesKHR& capabilities) const
{
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
    {
        return capabilities.currentExtent;
    }

    VkExtent2D actualExtent = mWindowExtent;
    actualExtent.width      = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
    actualExtent.height     = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

    return actualExtent;
}

} // Namespace Scarlett.
