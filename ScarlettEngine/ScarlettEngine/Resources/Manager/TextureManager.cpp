#include "ScarlettEnginePch.h"
#include "TextureManager.h"

#include <stb_image/stb_image.h>

#include "Rendering/Vulkan/VulkanUtils.h"
#include "Rendering/Vulkan/VulkanTexture.h"

namespace Scarlett
{

void TextureManager::Init(const WeakRef<Device> device)
{
    mRendererDevice = device;

    //todo we need to ensure that gpu accepts this number of max.
    constexpr uint32 MAX_TEXTURES            = 2048;
    constexpr uint32 NUM_TEXTURE_BINDINGS    = 1; // normal, albedo, metallic

    constexpr VkDescriptorSetLayoutBinding binding
    {
        .binding            = 0,
        .descriptorType     = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .descriptorCount    = MAX_TEXTURES,
        .stageFlags         = VK_SHADER_STAGE_FRAGMENT_BIT,
        .pImmutableSamplers = nullptr
    };

    VkDescriptorBindingFlags bindingFlags = VK_DESCRIPTOR_BINDING_VARIABLE_DESCRIPTOR_COUNT_BIT | VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT;

    const VkDescriptorSetLayoutBindingFlagsCreateInfo bindingFlagsInfo
    {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO,
        .bindingCount   = 1,
        .pBindingFlags  = &bindingFlags
    };

    const VkDescriptorSetLayoutCreateInfo layoutInfo
    {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .pNext          = &bindingFlagsInfo,
        .bindingCount   = 1,
        .pBindings      = &binding
    };

    VK_CHECK(vkCreateDescriptorSetLayout(mRendererDevice->GetDevice(), &layoutInfo, nullptr, &mDescriptorSetLayout), "Failed to create vulkan descriptor set layout");

    constexpr VkDescriptorPoolSize poolSize
    {
        .type               = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .descriptorCount    = MAX_TEXTURES
    };

    const VkDescriptorPoolCreateInfo poolInfo
    {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        .flags          = 0,
        .maxSets        = 1,
        .poolSizeCount  = 1,
        .pPoolSizes     = &poolSize,
    };

    VK_CHECK(vkCreateDescriptorPool(mRendererDevice->GetDevice(), &poolInfo, nullptr, &mDescriptorSetPool), "Failed to create vulkan descriptor set pool");

    // Add constant textures for the engine.
    // -------------------------------------------------------------------------------------------------------------------
    // White texture. [This texture is first so that all textures by default are plain white].
    constexpr std::array<uint8, 16> whiteTexture = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
    mLoadedTextures.emplace_back(new VulkanTexture{ mRendererDevice, whiteTexture.data(), 2, 2 });
    // Invalid texture.
    constexpr std::array<uint8, 16> invalidTexture = { 0xff, 0x0, 0xff, 0xff, 0xff, 0x0, 0xff, 0xff,  0xff, 0x0, 0xff, 0xff,  0xff, 0x0, 0xff, 0xff };
    mLoadedTextures.emplace_back(new VulkanTexture{ mRendererDevice, invalidTexture.data(), 2, 2 });
    // -------------------------------------------------------------------------------------------------------------------

    constexpr uint32 actualCount = MAX_TEXTURES;

    VkDescriptorSetVariableDescriptorCountAllocateInfo countInfo
    {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_ALLOCATE_INFO,
        .descriptorSetCount = 1,
        .pDescriptorCounts = &actualCount
    };

    const VkDescriptorSetAllocateInfo allocInfo
    {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .pNext                  = &countInfo,
        .descriptorPool         = mDescriptorSetPool,
        .descriptorSetCount     = 1,
        .pSetLayouts            = &mDescriptorSetLayout
    };

    VK_CHECK(vkAllocateDescriptorSets(mRendererDevice->GetDevice(), &allocInfo, &mDescriptorSet), "Failed to allocate descriptor sets");

    UpdateDescriptorSets(0, mLoadedTextures.size());
}

void TextureManager::Destroy()
{
    for (const Texture* texture : mLoadedTextures)
    {
        delete texture;
    }

    vkDestroyDescriptorSetLayout(mRendererDevice->GetDevice(), mDescriptorSetLayout, nullptr);
    vkDestroyDescriptorPool(mRendererDevice->GetDevice(), mDescriptorSetPool, nullptr);
}

textureId TextureManager::AddTexture(const Filepath& filepath, const bool updateDescriptorSets /* = true */)
{
    if (!filepath.DoesPathExist())
    {
        SCARLETT_ELOG("Failed to create texture since path does not exist: '{0}'", filepath.GetAbsolutePath());
        return INVALID_TEXTURE_INDEX;
    }

    // Check if the texture at the given filepath is already loaded, and if so, return that ID.
    if (mFilepathToTextureId.contains(filepath))
    {
        return mFilepathToTextureId[filepath];
    }

    stbi_set_flip_vertically_on_load(true);

    constexpr int requiredChannels = 4;

    int x, y, n;
    const uint8* buffer = stbi_load(filepath.GetAbsolutePath().c_str(), &x, &y, &n, requiredChannels);

    if (buffer == nullptr || x < 0 || y < 0 || n < 0)
    {
        SCARLETT_ELOG("Failed to create texture at path: '{0}'", filepath.GetAbsolutePath());
        return INVALID_TEXTURE_INDEX;
    }

    mLoadedTextures.emplace_back(new VulkanTexture{ mRendererDevice, buffer, static_cast<uint32>(x), static_cast<uint32>(y) });

    const textureId index = static_cast<textureId>(mLoadedTextures.size()) - 1;
    mFilepathToTextureId[filepath] = index;

    stbi_image_free((void*)buffer);

    if (updateDescriptorSets)
    {
        UpdateDescriptorSets(index, 1);
    }

    return index;
}

void TextureManager::UpdateDescriptorSets(const uint32 startingSlot, const uint32 count)
{
    const uint32 actualCount = static_cast<uint32>(mLoadedTextures.size());

    // Gather all VkDescriptorImageInfo from TextureManager for the textures you want to bind
    std::vector<VkDescriptorImageInfo> imageInfos;
    for (Texture* texture : mLoadedTextures)
    {
        const VulkanTexture* tex = dynamic_cast<VulkanTexture*>(texture);
        imageInfos.push_back(tex->GetImageInfo());
    }

    const VkWriteDescriptorSet write =
    {
        .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        .dstSet             = mDescriptorSet,
        .dstBinding         = 0,
        .dstArrayElement    = 0,
        .descriptorCount    = actualCount,
        .descriptorType     = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .pImageInfo         = imageInfos.data()
    };

    vkUpdateDescriptorSets(mRendererDevice->GetDevice(), 1, &write, 0, nullptr);
}

} // Namespace Scarlett.