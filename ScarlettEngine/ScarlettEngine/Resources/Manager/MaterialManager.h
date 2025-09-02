#pragma once

#include <vulkan/vulkan.h>

#include <Components/Material.h>

#include <ulid/ulid.hh>

#include "Rendering/Vulkan/Device.h"

namespace Scarlett
{

class Device;

class MaterialManager
{
public:
    MaterialManager() = default;
    ~MaterialManager() = default;

    //void Init(WeakRef<Device> device, VkDescriptorPool descriptorPool, VkDescriptorSetLayout descriptorSetLayout);
    void Init();
    void Destroy();

    uint32 LoadTexture(const Filepath& filePath);

    uint32 CreateMaterial(const Material& material);

    void CreateMaterial(const ulid::ULID& ulid, const Filepath& albedo);

    // Get Vulkan descriptor set with all textures bound
    VkDescriptorSet* GetTextureDescriptorSet() { return &mTextureDescriptorSet; }

    // Get material by index
    [[nodiscard]] inline const Material& GetMaterial(const uint32_t index) const { return mMaterials[index]; }

    // Helpers
    void UpdateDescriptorSet();
private:
    //WeakRef<Device> mDevice;

    // The descriptor set holding the big texture array
    VkDescriptorSet mTextureDescriptorSet;

    // List of all VkImageView and VkSampler pairs for textures
    vector<VkDescriptorImageInfo> mTextureImageInfos;

    // Materials
    vector<Material> mMaterials;

    // Descriptor pool and layout for allocating descriptor sets
    VkDescriptorPool        mDescriptorPool;
    VkDescriptorSetLayout   mDescriptorSetLayout;

};

} // Namespace Scarlett.
