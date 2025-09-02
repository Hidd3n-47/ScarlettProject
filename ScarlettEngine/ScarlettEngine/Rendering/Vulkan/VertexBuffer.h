#pragma once

#include "Device.h"

namespace Scarlett {

class VertexBuffer
{
public:
    explicit VertexBuffer(const WeakRef<Device> device, const uint64 bufferSize, const void* data);
    ~VertexBuffer();

    VertexBuffer(const VertexBuffer&)               = delete;
    VertexBuffer(VertexBuffer&&)                    = delete;
    VertexBuffer& operator=(VertexBuffer&&)         = delete;
    VertexBuffer& operator=(const VertexBuffer&)    = delete;

    void Bind(const VkCommandBuffer commandBuffer) const;
private:
    WeakRef<Device> mDevice;

    VkBuffer        mVertexBuffer;
    VkDeviceMemory  mVertexBufferMemory;
};

} // Namespace Scarlett.
