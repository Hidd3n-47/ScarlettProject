#pragma once

#include "Device.h"

namespace Scarlett
{

class Device;

class IndexBuffer
{
public:
    explicit IndexBuffer(Device* device, const uint32 indexCount, const void* data);
    ~IndexBuffer();

    IndexBuffer(const IndexBuffer&)               = delete;
    IndexBuffer(IndexBuffer&&)                    = delete;
    IndexBuffer& operator=(IndexBuffer&&)         = delete;
    IndexBuffer& operator=(const IndexBuffer&)    = delete;

    void Bind(const VkCommandBuffer commandBuffer) const;

    [[nodiscard]] inline uint32 GetNumberOfIndices() const { return mIndexCount; }
private:
    Device* mDevice;

    VkBuffer        mIndexBuffer;
    VkDeviceMemory  mIndexBufferMemory;
    uint32          mIndexCount;
};

} // Namespace Scarlett.
