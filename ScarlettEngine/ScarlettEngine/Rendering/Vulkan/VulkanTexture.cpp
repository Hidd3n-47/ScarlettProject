#include "ScarlettEnginePch.h"
#include "VulkanTexture.h"

#include "Device.h"
#include "Utilities.h"
#include "VulkanUtils.h"

#include <stb_image/stb_image.h>

namespace Scarlett
{

VulkanTexture::~VulkanTexture()
{
    vkDestroySampler(mDevice->GetDevice(), mImageSampler, nullptr);
    vkDestroyImageView(mDevice->GetDevice(), mImageView, nullptr);
    vkDestroyImage(mDevice->GetDevice(), mImage, nullptr);
    vkFreeMemory(mDevice->GetDevice(), mMemory, nullptr);
}


void VulkanTexture::Create(const Filepath& filepath)
{
    stbi_set_flip_vertically_on_load(1);

    int x, y, n;
    const uint8* buffer = stbi_load(filepath.GetAbsolutePath().c_str(), &x, &y, &n, 4);

    Create(buffer, x, y);
}

    //todo make it that you can pass in a texture
void VulkanTexture::Create(const uint8* buffer, const uint32 width, const uint32 height)
{
    VkCommandBuffer commandBuffer = BeginCommandBuffer(mDevice->GetDevice(), mDevice->GetCommandPool());

    mWidth  = width;
    mHeight = height;

    // todo change to have different formats instead of just hardcoded rgba (4).
    const size_t uploadSize = static_cast<size_t>(mWidth) * static_cast<size_t>(mHeight) * 4;

    const VkExtent3D imageExtent
    {
        .width = width,
        .height = height,
        .depth = 1
    };

    const VkImageCreateInfo info
    {
        .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .imageType      = VK_IMAGE_TYPE_2D,
        .format         = VK_FORMAT_R8G8B8A8_UNORM,
        .extent         = imageExtent,
        .mipLevels      = 1,
        .arrayLayers    = 1,
        .samples        = VK_SAMPLE_COUNT_1_BIT,
        .tiling         = VK_IMAGE_TILING_OPTIMAL,
        .usage          = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
        .sharingMode    = VK_SHARING_MODE_EXCLUSIVE,
        .initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED
    };

    mDevice->CreateImageWithInfo(info, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, mImage, mMemory);

    constexpr VkImageSubresourceRange subresourceRange
    {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel   = 0,
            .levelCount     = 1,
            .baseArrayLayer = 0,
            .layerCount     = 1
    };

    const VkImageViewCreateInfo viewInfo
    {
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .image              = mImage,
        .viewType           = VK_IMAGE_VIEW_TYPE_2D,
        .format             = VK_FORMAT_R8G8B8A8_UNORM,
        .subresourceRange   = subresourceRange
    };

    VK_CHECK(vkCreateImageView(mDevice->GetDevice(), &viewInfo, nullptr, &mImageView), "Failed to create Vulkan Texture Image View.");

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    mDevice->CreateBuffer(uploadSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, stagingBuffer, stagingBufferMemory);

    uint8* map = nullptr;
    VK_CHECK(vkMapMemory(mDevice->GetDevice(), stagingBufferMemory, 0, uploadSize, 0, reinterpret_cast<void**>(&map)), "Filed to map Vulkan Image Memory.");

    memcpy(map, buffer, uploadSize); //todo this is where the buffer should go for the texture "pixels".
    const VkMappedMemoryRange range =
    {
        .sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,
        .memory = stagingBufferMemory,
        .size   = uploadSize
    };
    VK_CHECK(vkFlushMappedMemoryRanges(mDevice->GetDevice(), 1, &range), "Failed to Flush Vulkan Mapped Memory Ranges.");
    vkUnmapMemory(mDevice->GetDevice(), stagingBufferMemory);

    VkImageMemoryBarrier imageCopyBarrier
    {
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .dstAccessMask          = VK_ACCESS_TRANSFER_WRITE_BIT,
        .oldLayout              = VK_IMAGE_LAYOUT_UNDEFINED,
        .newLayout              = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        .srcQueueFamilyIndex    = VK_QUEUE_FAMILY_IGNORED,
        .dstQueueFamilyIndex    = VK_QUEUE_FAMILY_IGNORED,
        .image                  = mImage,
        .subresourceRange       = subresourceRange
    };
    vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_HOST_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &imageCopyBarrier);
    constexpr VkImageSubresourceLayers imageSubResource
    {
        .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
        .layerCount = 1
    };

    VkBufferImageCopy region
    {
        .imageSubresource = imageSubResource,
        .imageExtent = imageExtent
    };
    vkCmdCopyBufferToImage(commandBuffer, stagingBuffer, mImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

    VkImageMemoryBarrier useBarrier
    {
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .srcAccessMask          = VK_ACCESS_TRANSFER_WRITE_BIT,
        .dstAccessMask          = VK_ACCESS_SHADER_READ_BIT,
        .oldLayout              = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        .newLayout              = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
        .srcQueueFamilyIndex    = VK_QUEUE_FAMILY_IGNORED,
        .dstQueueFamilyIndex    = VK_QUEUE_FAMILY_IGNORED,
        .image                  = mImage,
        .subresourceRange       = subresourceRange
    };
    vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &useBarrier);

    EndAndSubmitCommandBuffer(mDevice->GetDevice(), mDevice->GetCommandPool(), mDevice->GetGraphicsQueue(), commandBuffer);

    vkDestroyBuffer(mDevice->GetDevice(), stagingBuffer, nullptr);
    vkFreeMemory(mDevice->GetDevice(), stagingBufferMemory, nullptr);

    VkSamplerCreateInfo samplerInfo
    {
        .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
        .magFilter = VK_FILTER_LINEAR,
        .minFilter = VK_FILTER_LINEAR,
        .mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,
        .addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT,
        .addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT,
        .addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT,
        .maxAnisotropy = 1.0f,
        .minLod = -1000,
        .maxLod = 1000
    };

    VK_CHECK(vkCreateSampler(mDevice->GetDevice(), &samplerInfo, nullptr, &mImageSampler), "Failed to create Vulkan Sampler for the texture.");

    mImageInfo =
    {
        .sampler        = mImageSampler,
        .imageView      = mImageView,
        .imageLayout    = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
    };
}

} // Namespace Scarlett.
