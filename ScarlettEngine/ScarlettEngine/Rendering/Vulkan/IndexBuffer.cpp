#include "ScarlettEnginePch.h"
#include "IndexBuffer.h"

namespace Scarlett
{

IndexBuffer::IndexBuffer(Device* device, const uint32 indexCount, const void* data)
    : mDevice { device }
    , mIndexCount { indexCount }
{
    // Todo: Implement staging buffer -> See vertex buffer.
    const uint32 bufferSize = indexCount * sizeof(uint32);

    mDevice->CreateBuffer(bufferSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, mIndexBuffer, mIndexBufferMemory);

    void* dataDst;
    vkMapMemory(mDevice->GetDevice(), mIndexBufferMemory, 0, bufferSize, 0, &dataDst);
    memcpy(dataDst, data, bufferSize);
    vkUnmapMemory(mDevice->GetDevice(), mIndexBufferMemory);
}

IndexBuffer::~IndexBuffer()
{
    vkDeviceWaitIdle(mDevice->GetDevice());
    vkDestroyBuffer(mDevice->GetDevice(), mIndexBuffer, nullptr);
    vkFreeMemory(mDevice->GetDevice(), mIndexBufferMemory, nullptr);
}

void IndexBuffer::Bind(const VkCommandBuffer commandBuffer) const
{
    vkCmdBindIndexBuffer(commandBuffer, mIndexBuffer, 0, VK_INDEX_TYPE_UINT32);
}

} // Namespace Scarlett.
