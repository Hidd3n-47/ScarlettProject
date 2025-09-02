#pragma once

#include "Device.h"
#include "Rendering/Texture.h"

namespace Scarlett
{

class VulkanTexture final : public Texture
{
public:
    VulkanTexture(const WeakRef<Device> device, const uint8* buffer, const uint32 width, const uint32 height);
    ~VulkanTexture() override;

    VulkanTexture(const VulkanTexture&)               = delete;
    VulkanTexture(VulkanTexture&&)                    = delete;
    VulkanTexture& operator=(VulkanTexture&&)         = delete;
    VulkanTexture& operator=(const VulkanTexture&)    = delete;

    [[nodiscard]] inline const VkDescriptorImageInfo& GetImageInfo() const { return mImageInfo; }
protected:
    void Create(const uint8* buffer, const uint32 width, const uint32 height) override;
private:
    WeakRef<Device> mDevice;

    VkImage         mImage              = nullptr;
    VkDeviceMemory  mMemory             = nullptr;
    VkImageView     mImageView          = nullptr;

    VkSampler mImageSampler;

    VkDescriptorImageInfo mImageInfo{};
};

} // Namespace Scarlett.
