#include "ScarlettEnginePch.h"
#include "MaterialManager.h"


#include "TextureManager.h"
#include "Core/Engine.h"
#include "Rendering/Vulkan/VulkanTexture.h"

namespace Scarlett
{

//void MaterialManager::Init(WeakRef<Device> device, VkDescriptorPool descriptorPool, VkDescriptorSetLayout descriptorSetLayout)
void MaterialManager::Init()
{
    //mDevice = device;
    //mDescriptorPool = descriptorPool;
    //mDescriptorSetLayout = descriptorSetLayout;
    //uint32_t descriptorCount = 2048; // or current texture count

    //VkDescriptorSetVariableDescriptorCountAllocateInfo variableDescCountInfo{
    //    .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_ALLOCATE_INFO,
    //    .descriptorSetCount = 1,
    //    .pDescriptorCounts = &descriptorCount
    //};

    //VkDescriptorSetAllocateInfo allocInfo{};
    //allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    //allocInfo.descriptorPool = mDescriptorPool;
    //allocInfo.descriptorSetCount = 2048;
    //allocInfo.pSetLayouts = &mDescriptorSetLayout;
    //allocInfo.pNext = &variableDescCountInfo;

    //vkAllocateDescriptorSets(mDevice->GetDevice(), &allocInfo, &mTextureDescriptorSet);
    uint32_t initialTextureCount = 1;

    VkDescriptorSetVariableDescriptorCountAllocateInfo variableCountInfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_ALLOCATE_INFO,
        .descriptorSetCount = 1,
        .pDescriptorCounts = &initialTextureCount
    };

    VkDescriptorSetAllocateInfo allocInfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .pNext = nullptr,
        .descriptorPool = mDescriptorPool,
        .descriptorSetCount = 1,
        .pSetLayouts = &mDescriptorSetLayout,
    };

    //VkResult result = vkAllocateDescriptorSets(mDevice->GetDevice(), &allocInfo, &mTextureDescriptorSet);
    //if (result != VK_SUCCESS) {
        SCARLETT_ELOG("Failed to allocate descriptor set for material textures.");
    //}

}


void MaterialManager::Destroy()
{
}

uint32 MaterialManager::LoadTexture(const Filepath& filePath)
{
    // 1. Load textures via TextureManager (example: only albedo for now)
    uint32 albedoTextureIndex = Engine::Instance().GetTextureManager()->AddTexture(filePath);

    // 2. Create a new Material object that stores texture indices and other data
    Material newMaterial;
    newMaterial.albedoTextureIndex = albedoTextureIndex;
    // ... store other material info here

    uint32 newMaterialIndex = static_cast<uint32>(mMaterials.size());
    mMaterials.push_back(newMaterial);

    // 3. Update descriptor sets with new texture image info
    UpdateDescriptorSet();

    return newMaterialIndex;
}

uint32 MaterialManager::CreateMaterial(const Material& material)
{
    return 0;
}

void MaterialManager::CreateMaterial(const ulid::ULID& ulid, const Filepath& albedo)
{
    // load the albedo texture
    LoadTexture(albedo);

    // then register the material into the material manager.
}

void MaterialManager::UpdateDescriptorSet()
{
    // Gather all VkDescriptorImageInfo from TextureManager for the textures you want to bind
    std::vector<VkDescriptorImageInfo> imageInfos;
    for (Texture* texture : Engine::Instance().GetTextureManager()->mLoadedTextures)
    {
        VulkanTexture* tex = dynamic_cast<VulkanTexture*>(texture);
        imageInfos.push_back(tex->GetImageInfo());
    }

    const VkWriteDescriptorSet wds
    {
        .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        .dstSet             = mTextureDescriptorSet,
        .dstBinding         = 0,
        .dstArrayElement    = 0,
        .descriptorCount    =  static_cast<uint32_t>(imageInfos.size()),
        .descriptorType     = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .pImageInfo         = imageInfos.data(),
    };

    //vkUpdateDescriptorSets(mDevice->GetDevice(), 1, &wds, 0, nullptr);
}


} // Namespace Scarlett.
