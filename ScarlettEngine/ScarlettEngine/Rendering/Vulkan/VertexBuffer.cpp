#include "ScarlettEnginepch.h"
#include "VertexBuffer.h"

namespace Scarlett
{

VertexBuffer::VertexBuffer(Device* device, const uint64 bufferSize, const void* data)
    : mDevice { device }
{
    mDevice->CreateBuffer(bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, mVertexBuffer, mVertexBufferMemory);

    void* dataDst;
    vkMapMemory(mDevice->GetDevice(), mVertexBufferMemory, 0, bufferSize, 0, &dataDst);
    memcpy(dataDst, data, bufferSize);
    vkUnmapMemory(mDevice->GetDevice(), mVertexBufferMemory);
}

VertexBuffer::~VertexBuffer()
{
    vkDeviceWaitIdle(mDevice->GetDevice());
    vkDestroyBuffer(mDevice->GetDevice(), mVertexBuffer, nullptr);
    vkFreeMemory(mDevice->GetDevice(), mVertexBufferMemory, nullptr);
}

void VertexBuffer::Bind(VkCommandBuffer commandBuffer) const
{
    const VkBuffer buffers[] = { mVertexBuffer };
    constexpr VkDeviceSize offsets[] = { 0 };

    vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
}

} // Namespace Scarlett.
