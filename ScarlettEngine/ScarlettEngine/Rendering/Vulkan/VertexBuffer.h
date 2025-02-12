#pragma once

#include "Device.h"

namespace Scarlett {

class VertexBuffer
{
public:
    VertexBuffer(Device* device, const uint64 bufferSize, const void* data);
    ~VertexBuffer();

    void Bind(VkCommandBuffer commandBuffer) const;
private:
    Device* mDevice;

    VkBuffer        mVertexBuffer;
    VkDeviceMemory  mVertexBufferMemory;
};

} // Namespace Scarlett.