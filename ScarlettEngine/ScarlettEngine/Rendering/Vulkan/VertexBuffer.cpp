#include "ScarlettEnginepch.h"
#include "VertexBuffer.h"

namespace Scarlett
{

VertexBuffer::VertexBuffer(const WeakRef<Device> device, const uint64 bufferSize, const void* data)
    : mDevice { device }
{
    mDevice->CreateBuffer(bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, mVertexBuffer, mVertexBufferMemory);

    void* dataDst;
    vkMapMemory(mDevice->GetDevice(), mVertexBufferMemory, 0, bufferSize, 0, &dataDst);
    memcpy(dataDst, data, bufferSize);
    vkUnmapMemory(mDevice->GetDevice(), mVertexBufferMemory);

    // Todo: Implement staging buffer.
    //---------------------------------
    // VkBuffer stagingBuffer;
    // VkDeviceMemory stagingBufferMemory;
    // mDevice->CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);
    //
    // void* dataDst;
    // vkMapMemory(mDevice->GetDevice(), stagingBufferMemory, 0, bufferSize, 0, &dataDst);
    // memcpy(dataDst, data, bufferSize);
    // vkUnmapMemory(mDevice->GetDevice(), stagingBufferMemory);
    //
    // mDevice->CreateBuffer(bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, mVertexBuffer, mVertexBufferMemory);
    // mDevice->CopyBuffer(stagingBuffer, mVertexBuffer, bufferSize);
}

VertexBuffer::~VertexBuffer()
{
    vkDeviceWaitIdle(mDevice->GetDevice());
    vkDestroyBuffer(mDevice->GetDevice(), mVertexBuffer, nullptr);
    vkFreeMemory(mDevice->GetDevice(), mVertexBufferMemory, nullptr);
}

void VertexBuffer::Bind(const VkCommandBuffer commandBuffer) const
{
    constexpr VkDeviceSize offsets[] = { 0 };

    vkCmdBindVertexBuffers(commandBuffer, 0, 1, &mVertexBuffer, offsets);
}

} // Namespace Scarlett.
