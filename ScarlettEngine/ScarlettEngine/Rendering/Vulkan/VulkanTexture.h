#pragma once

#include <vulkan/vulkan.h>

#include "Device.h"
#include "Rendering/Texture.h"

namespace Scarlett
{

class VulkanTexture final : public Texture
{
public:
    ~VulkanTexture() override;

    void Create(const Filepath& filepath) override;
    void Create(const uint8* buffer, const uint32 width, const uint32 height) override;

    [[nodiscard]] inline const VkDescriptorImageInfo& GetImageInfo() const { return mImageInfo; }
    inline void SetDevice(Device* device) { mDevice = device; }
private:
    Device* mDevice = nullptr;

    VkImage         mImage          = nullptr;
    VkDeviceMemory  mMemory         = nullptr;
    VkImageView     mImageView      = nullptr;
    VkSampler       mImageSampler   = nullptr;

    VkDescriptorImageInfo mImageInfo{};
};

} // Namespace Scarlett.
