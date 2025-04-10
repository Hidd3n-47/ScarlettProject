#pragma once

#include "ScarlettEnginepch.h"

#include <vulkan/vulkan.h>

const vector<const char*> gDeviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

// Indices of Queue Families (if they exist)
struct QueueFamilyIndices
{
    int graphicsFamily = -1;
    int presentationFamily = -1;

    inline bool IsValid() const { return graphicsFamily >= 0 && presentationFamily >= 0; }
};

struct SwapChainDetails
{
    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    vector<VkSurfaceFormatKHR> formats;
    vector<VkPresentModeKHR> presentationModes;
};

struct SwapChainImage
{
    VkImage image;
    VkImageView imageView;
};

uint32 static FindMemoryTypeIndex(const VkPhysicalDevice physicalDevice, const uint32 allowedTypes, const VkMemoryPropertyFlags propertyFlags)
{
    VkPhysicalDeviceMemoryProperties memoryProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);

    for (uint32 i = 0; i < memoryProperties.memoryTypeCount; ++i)
    {
        if ((allowedTypes & (1 << i)) && (memoryProperties.memoryTypes[i].propertyFlags & propertyFlags) == propertyFlags)
        {
            return i;
        }
    }

    SCARLETT_ASSERT(false && "Failed to find memory type with passed in property flags.");
    return -1;
}

static VkCommandBuffer BeginCommandBuffer(const VkDevice device, const VkCommandPool pool)
{
    VkCommandBuffer commandBuffer;

    VkCommandBufferAllocateInfo commandBufferInfo = {};
    commandBufferInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    commandBufferInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    commandBufferInfo.commandBufferCount = 1;
    commandBufferInfo.commandPool = pool;

    vkAllocateCommandBuffers(device, &commandBufferInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
}

static void EndAndSubmitCommandBuffer(const VkDevice device, const VkCommandPool pool, const VkQueue queue, const VkCommandBuffer commandBuffer)
{
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(queue);

    vkFreeCommandBuffers(device, pool, 1, &commandBuffer);
}

static void CopyBuffer(VkDevice logicalDevice, VkQueue transferQueue, VkCommandPool transferCommandPool, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize bufferSize)
{
    VkCommandBuffer transferCommandBuffer = BeginCommandBuffer(logicalDevice, transferCommandPool);

    VkBufferCopy copyRegion = {};
    copyRegion.srcOffset = 0;
    copyRegion.dstOffset = 0;
    copyRegion.size = bufferSize;

    vkCmdCopyBuffer(transferCommandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    EndAndSubmitCommandBuffer(logicalDevice, transferCommandPool, transferQueue, transferCommandBuffer);
}

static void CopyImageBuffer(VkDevice logicalDevice, VkQueue transferQueue, VkCommandPool transferCommandPool, VkBuffer srcBuffer, VkImage image, uint32 width, uint32 height)
{
    VkCommandBuffer transferCommandBuffer = BeginCommandBuffer(logicalDevice, transferCommandPool);

    VkBufferImageCopy imageRegion = {};
    imageRegion.bufferOffset = 0;
    imageRegion.bufferRowLength = 0;
    imageRegion.bufferImageHeight = 0;
    imageRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    imageRegion.imageSubresource.mipLevel = 0;
    imageRegion.imageSubresource.baseArrayLayer = 0;
    imageRegion.imageSubresource.layerCount = 1;
    imageRegion.imageOffset = { 0, 0, 0 };
    imageRegion.imageExtent = { width, height, 1 };

    vkCmdCopyBufferToImage(transferCommandBuffer, srcBuffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &imageRegion);

    EndAndSubmitCommandBuffer(logicalDevice, transferCommandPool, transferQueue, transferCommandBuffer);
}

static void TransitionImageLayout(VkDevice logicalDevice, VkQueue queue, VkCommandPool commandPool, VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout)
{
    VkCommandBuffer commandBuffer = BeginCommandBuffer(logicalDevice, commandPool);

    VkImageMemoryBarrier imageMemoryBarrier = {};
    imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    imageMemoryBarrier.oldLayout = oldLayout;
    imageMemoryBarrier.newLayout = newLayout;
    imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    imageMemoryBarrier.image = image;
    imageMemoryBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    imageMemoryBarrier.subresourceRange.baseMipLevel = 0;
    imageMemoryBarrier.subresourceRange.levelCount = 1;
    imageMemoryBarrier.subresourceRange.baseArrayLayer = 0;
    imageMemoryBarrier.subresourceRange.layerCount = 1;

    VkPipelineStageFlags srcStage;
    VkPipelineStageFlags dstStage;

    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
    {
        imageMemoryBarrier.srcAccessMask = 0;
        imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        dstStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
    {
        imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        srcStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        dstStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }

    vkCmdPipelineBarrier(commandBuffer, srcStage, dstStage, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);

    EndAndSubmitCommandBuffer(logicalDevice, commandPool, queue, commandBuffer);
}
