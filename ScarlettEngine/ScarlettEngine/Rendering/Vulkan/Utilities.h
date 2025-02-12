#pragma once

#include "ScarlettEnginepch.h"

#include <vulkan/vulkan.h>

#include <glm/glm.hpp>

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

static vector<char> ReadFile(const std::string& fileName)
{
    std::ifstream fin(fileName, std::ios::binary | std::ios::ate);

    if(!fin.is_open())
    {
        SCARLETT_ELOG("Failed to open file at path: {0}", fileName);
        throw std::runtime_error("Failed to open file at path: " + fileName);
    }

    size_t fileSize = (size_t)fin.tellg();
    vector<char> fileBuffer(fileSize);

    fin.seekg(0);

    fin.read(fileBuffer.data(), fileSize);

    fin.close();

    return fileBuffer;
}

uint32 static FindMemoryTypeIndex(VkPhysicalDevice physicalDevice, uint32 allowedTypes, VkMemoryPropertyFlags propertyFlags)
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

static void CreateBuffer(VkPhysicalDevice physicalDevice, VkDevice logicalDevice, VkDeviceSize bufferSize, VkBufferUsageFlags bufferUsageFlags, VkMemoryPropertyFlags bufferProperties, VkBuffer* buffer, VkDeviceMemory* bufferMemory)
{
    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = bufferSize;
    bufferInfo.usage = bufferUsageFlags;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VkResult result = vkCreateBuffer(logicalDevice, &bufferInfo, nullptr, buffer);
    if (result != VK_SUCCESS)
    {
        SCARLETT_FLOG("Failed to create a vulkan vertex buffer. Error code: {0}", (int)result);
        throw std::runtime_error("Failed to create a vulkan vertex buffer.");
    }

    VkMemoryRequirements memoryRequirements;
    vkGetBufferMemoryRequirements(logicalDevice, *buffer, &memoryRequirements);

    VkMemoryAllocateInfo memoryAllocInfo = {};
    memoryAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memoryAllocInfo.allocationSize = memoryRequirements.size;
    memoryAllocInfo.memoryTypeIndex = FindMemoryTypeIndex(physicalDevice, memoryRequirements.memoryTypeBits, bufferProperties);

    result = vkAllocateMemory(logicalDevice, &memoryAllocInfo, nullptr, bufferMemory);
    if (result != VK_SUCCESS)
    {
        SCARLETT_FLOG("Failed to allocate vertex buffer memory (vulkan). Error code: {0}", (int)result);
        throw std::runtime_error("Failed to allocate vertex buffer memory (vulkan).");
    }

    vkBindBufferMemory(logicalDevice, *buffer, *bufferMemory, 0);
}

static VkCommandBuffer BeginCommandBuffer(VkDevice device, VkCommandPool pool)
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

static void EndAndSubmitCommandBuffer(VkDevice device, VkCommandPool pool, VkQueue queue, VkCommandBuffer commandBuffer)
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