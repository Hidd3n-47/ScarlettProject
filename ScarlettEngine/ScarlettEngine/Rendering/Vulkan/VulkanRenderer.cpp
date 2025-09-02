#include "ScarlettEnginepch.h"
#include "VulkanRenderer.h"

#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

#include <ScarlettGameCore/Src/GameCore.h>

#include <Components/Camera.h>

#include "VulkanMesh.h"
#include "VulkanUtils.h"
#include "VulkanTexture.h"
#include "Core/Engine.h"

#include "Core/Window/Window.h"
#include "Rendering/ModelInfo.h"
#include "Resources/Manager/TextureManager.h"

#include "Resources/Manager/MaterialManager.h"

namespace Scarlett
{

#ifndef SCARLETT_EDITOR_ENABLED
    std::unique_ptr<Renderer> Renderer::mInstance = std::make_unique<VulkanRenderer>();
#endif // SCARLETT_EDITOR_ENABLED.

void VulkanRenderer::Init(const Window* windowRef)
{
    mWindowRef = windowRef;

    try
    {
        mDevice.Init(windowRef);

        // Todo this is a part of ImGui set up so should somehow be restricted only for editor mode.
        // -----------------------------------------------------------------------------------
        /*constexpr uint32 MAX_COUNT = 1000;
        constexpr VkDescriptorPoolSize poolSizes[] =
        {
            { VK_DESCRIPTOR_TYPE_SAMPLER,                   MAX_COUNT },
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,    MAX_COUNT },
            { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,             MAX_COUNT },
            { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,             MAX_COUNT },
            { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,      MAX_COUNT },
            { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER,      MAX_COUNT },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,            MAX_COUNT },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,            MAX_COUNT },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,    MAX_COUNT },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC,    MAX_COUNT },
            { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,          MAX_COUNT }
        };

        const VkDescriptorPoolCreateInfo descriptorPoolInfo
        {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
            .flags          = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
            .maxSets        = MAX_COUNT,
            .poolSizeCount  = std::size(poolSizes),
            .pPoolSizes     = poolSizes
        };*/

        Engine::Instance().GetTextureManager()->Init(WeakRef{ &mDevice });

        constexpr uint32 MAX_TEXTURES = 2048;
        constexpr uint32 NUM_TEXTURE_BINDINGS = 1; // normal, albeido, metallic

        /*constexpr VkSamplerCreateInfo samplerInfo
        {
            .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
            .magFilter      = VK_FILTER_LINEAR,
            .minFilter      = VK_FILTER_LINEAR,
            .mipmapMode     = VK_SAMPLER_MIPMAP_MODE_LINEAR,
            .addressModeU   = VK_SAMPLER_ADDRESS_MODE_REPEAT,
            .addressModeV   = VK_SAMPLER_ADDRESS_MODE_REPEAT,
            .addressModeW   = VK_SAMPLER_ADDRESS_MODE_REPEAT,
            .maxAnisotropy  = 1.0f,
            .minLod         = -1000,
            .maxLod         = 1000
        };

        VK_CHECK(vkCreateSampler(mDevice.GetDevice(), &samplerInfo, nullptr, &mImageSampler), "Failed to create Vulkan Sampler for the texture.");*/

        //std::vector<VkSampler> samplers(MAX_TEXTURES, mImageSampler);

        const VkDescriptorSetLayoutBinding binding
        {
            .binding = 0,
            .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            .descriptorCount = MAX_TEXTURES, // max possible descriptors you might bind
            .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
            .pImmutableSamplers = nullptr
        };

        VkDescriptorBindingFlags bindingFlags = VK_DESCRIPTOR_BINDING_VARIABLE_DESCRIPTOR_COUNT_BIT | VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT;

        VkDescriptorSetLayoutBindingFlagsCreateInfo bindingFlagsInfo
        {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO,
            .bindingCount = 1,
            .pBindingFlags = &bindingFlags
        };

        VkDescriptorSetLayoutCreateInfo layoutInfo = {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
            .pNext = &bindingFlagsInfo,
            .bindingCount = 1,
            .pBindings = &binding
        };

        VK_CHECK(vkCreateDescriptorSetLayout(mDevice.GetDevice(), &layoutInfo, nullptr, &mDescriptorSetLayout), "Failed to create vulkan descriptor set layout");


        VkDescriptorPoolSize poolSize
        {
            .type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            .descriptorCount = MAX_TEXTURES
        };

        VkDescriptorPoolCreateInfo poolInfo = {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
            .flags = 0,
            .maxSets = 1,
            .poolSizeCount = 1,
            .pPoolSizes = &poolSize,
        };

        VK_CHECK(vkCreateDescriptorPool(mDevice.GetDevice(), &poolInfo, nullptr, &mDescriptorSetPool), "Failed to create vulkan descriptor set pool");

        uint32 actualCount = static_cast<uint32>(Engine::Instance().GetTextureManager()->mLoadedTextures.size());

        VkDescriptorSetVariableDescriptorCountAllocateInfo countInfo
        {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_ALLOCATE_INFO,
            .descriptorSetCount = 1,
            .pDescriptorCounts = &actualCount
        };

        VkDescriptorSetAllocateInfo allocInfo
        {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
            .pNext = &countInfo,
            .descriptorPool = mDescriptorSetPool,
            .descriptorSetCount = 1,
            .pSetLayouts = &mDescriptorSetLayout
        };

        VK_CHECK(vkAllocateDescriptorSets(mDevice.GetDevice(), &allocInfo, &mDescriptorSet), "Failed to allocate descriptor sets");


        // Gather all VkDescriptorImageInfo from TextureManager for the textures you want to bind
        std::vector<VkDescriptorImageInfo> imageInfos;
        for (Texture* texture : Engine::Instance().GetTextureManager()->mLoadedTextures)
        {
            VulkanTexture* tex = dynamic_cast<VulkanTexture*>(texture);
            imageInfos.push_back(tex->GetImageInfo());
        }

        VkWriteDescriptorSet write = {
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .dstSet = mDescriptorSet,
            .dstBinding = 0,
            .descriptorCount = actualCount,
            .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            .pImageInfo = imageInfos.data()
        };

        vkUpdateDescriptorSets(mDevice.GetDevice(), 1, &write, 0, nullptr);

        //constexpr uint32 MAX_TEXTURES = 2048;
        //constexpr uint32 NUM_TEXTURE_BINDINGS = 1; // normal, albeido, metallic

        //constexpr VkDescriptorPoolSize poolSize
        //{
        //    .type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        //    .descriptorCount = MAX_TEXTURES * NUM_TEXTURE_BINDINGS // e.g., 2048 * 3
        //};

        //VkDescriptorPoolCreateInfo poolInfo
        //{
        //    .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        //    .flags          = VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT,
        //    .maxSets        = 1,
        //    .poolSizeCount  = 1,
        //    .pPoolSizes     = &poolSize
        //};

        //VK_CHECK(vkCreateDescriptorPool(mDevice.GetDevice(), &poolInfo, nullptr, &mDescriptorSetPool), "Failed to create descriptor pool");

        //// Example for 3 bindings: albedo, normal, roughness
        //std::vector<VkDescriptorSetLayoutBinding> bindings = {
        //    {0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, MAX_TEXTURES, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr}, // albedo
        //    //{1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, MAX_TEXTURES, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr}, // normal
        //    //{2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, MAX_TEXTURES, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr}, // roughness
        //};

        //constexpr VkDescriptorBindingFlags bindingFlags = VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT | VK_DESCRIPTOR_BINDING_VARIABLE_DESCRIPTOR_COUNT_BIT | VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT;

        //const VkDescriptorSetLayoutBindingFlagsCreateInfo bindingFlagsInfo
        //{
        //    .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO,
        //    .bindingCount   = 1,
        //    .pBindingFlags  = &bindingFlags,
        //};

        //const VkDescriptorSetLayoutCreateInfo layoutInfo
        //{
        //    .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        //    .pNext          = &bindingFlagsInfo,
        //    .flags          = VK_DESCRIPTOR_SET_LAYOUT_CREATE_UPDATE_AFTER_BIND_POOL_BIT,
        //    .bindingCount   = static_cast<uint32_t>(bindings.size()),
        //    .pBindings      = bindings.data(),
        //};

        //vkCreateDescriptorSetLayout(mDevice.GetDevice(), &layoutInfo, nullptr, &textureSetLayout);

        //uint32 actualAlbedoCount = MAX_TEXTURES;//, actualNormalCount = 0, actualRoughnessCount = 0;
        //uint32_t variableDescriptorCounts[] =
        //{
        //    actualAlbedoCount,
        //    //actualNormalCount,
        //    //actualRoughnessCount,
        //};

        //VkDescriptorSetVariableDescriptorCountAllocateInfo variableDescriptorCountAllocInfo{};
        //variableDescriptorCountAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_ALLOCATE_INFO;
        //variableDescriptorCountAllocInfo.descriptorSetCount = 1;
        //variableDescriptorCountAllocInfo.pDescriptorCounts = variableDescriptorCounts;

        //VkDescriptorSetAllocateInfo allocInfo{};
        //allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        //allocInfo.descriptorPool = mDescriptorSetPool;
        //allocInfo.descriptorSetCount = 1;
        //allocInfo.pSetLayouts = &textureSetLayout;
        //allocInfo.pNext = &variableDescriptorCountAllocInfo;

        //vkAllocateDescriptorSets(mDevice.GetDevice(), &allocInfo, &textureDescriptorSetF);

        //constexpr VkSamplerCreateInfo samplerInfo
        //{
        //    .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
        //    .magFilter      = VK_FILTER_LINEAR,
        //    .minFilter      = VK_FILTER_LINEAR,
        //    .mipmapMode     = VK_SAMPLER_MIPMAP_MODE_LINEAR,
        //    .addressModeU   = VK_SAMPLER_ADDRESS_MODE_REPEAT,
        //    .addressModeV   = VK_SAMPLER_ADDRESS_MODE_REPEAT,
        //    .addressModeW   = VK_SAMPLER_ADDRESS_MODE_REPEAT,
        //    .maxAnisotropy  = 1.0f,
        //    .minLod         = -1000,
        //    .maxLod         = 1000
        //};

        //VK_CHECK(vkCreateSampler(mDevice.GetDevice(), &samplerInfo, nullptr, &mImageSampler), "Failed to create Vulkan Sampler for the texture.");

        //VkDescriptorSetVariableDescriptorCountAllocateInfo variableCountInfo
        //{
        //    .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_ALLOCATE_INFO,
        //    .descriptorSetCount = 1,
        //    .pDescriptorCounts = &MAX_TEXTURES
        //};

        //VkDescriptorSetAllocateInfo allocInfoSet
        //{
        //    .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        //    .pNext = nullptr,
        //    .descriptorPool = mDescriptorSetPool,
        //    .descriptorSetCount = 1,
        //    .pSetLayouts = &textureSetLayout
        //};

        //vkAllocateDescriptorSets(mDevice.GetDevice(), &allocInfoSet, &textureDescriptorSetF);

        CreatePipelineLayout();

        RecreateSwapChain(mWindowRef->GetWidth(), mWindowRef->GetHeight());
        CreatePipeline();

        CreateCommandBuffers();
    }
    catch(const std::runtime_error& e)
    {
        SCARLETT_FLOG("Error: {0}", e.what());
    }
}

void VulkanRenderer::Destroy()
{
    vkDeviceWaitIdle(mDevice.mDevice);

    delete mTexture;
    vkDestroyDescriptorSetLayout(mDevice.GetDevice(), mDescriptorSetLayout, nullptr);

    vkDestroyDescriptorPool(mDevice.GetDevice(), mDescriptorSetPool, nullptr);

    FreeCommandBuffers();

    mPipeline->Destroy();
    delete mPipeline;
    mPipeline = nullptr;
    vkDestroyPipelineLayout(mDevice.GetDevice(), mPipelineLayout, nullptr);

    SCARLETT_ASSERT(mSwapChain && "Trying to destroy a swapchain that was not created.");
    delete mSwapChain;
    mSwapChain= nullptr;

    mDevice.Destroy();
}

void VulkanRenderer::BeginRender()
{
    const VkResult result = mSwapChain->AcquireNextImage(&mNextImageIndex);

    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    {
        throw std::runtime_error("Failed to acquire swap chain image");
    }

    constexpr VkCommandBufferBeginInfo beginInfo
    {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .pNext              = VK_NULL_HANDLE,
        .flags              = 0,
        .pInheritanceInfo   = VK_NULL_HANDLE
    };

    VK_CHECK(vkBeginCommandBuffer(mCommandBuffers[mNextImageIndex], &beginInfo), "Failed to begin recording Vulkan Command Buffer");

    const VkRect2D renderArea
    {
        .offset = { 0, 0 },
        .extent = mSwapChain->GetSwapChainExtent()
    };

    std::array<VkClearValue, 2> clearValues;
    clearValues[0].color            = { .float32 = { mClearColor[0], mClearColor[1], mClearColor[2], 1.0f } };
    clearValues[1].depthStencil     = { .depth = 1.0f, .stencil = 0 };

    const VkRenderPassBeginInfo renderPassInfo
    {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        .pNext              = VK_NULL_HANDLE,
        .renderPass         = mSwapChain->GetRenderPass(),
        .framebuffer        = mSwapChain->GetFrameBuffer(static_cast<int>(mNextImageIndex)),
        .renderArea         = renderArea,
        .clearValueCount    = static_cast<uint32>(clearValues.size()),
        .pClearValues       = clearValues.data()
    };

    vkCmdBeginRenderPass(mCommandBuffers[mNextImageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void VulkanRenderer::Render()
{
    mPipeline->Bind(mCommandBuffers[mNextImageIndex]);
    RecordCommandBuffer(mNextImageIndex);
}

void VulkanRenderer::EndRender()
{
    const Component::Camera* camera = &ScarlettGame::GameCore::Instance().GetActiveScene()->GetComponentManager()->GetComponentArray<Component::Camera>()[0];

    // Sprites.
    // todo move towards a vertex and index buffer to batch/instance render.
    //vector<Vertex> mVertexBuffer;
    //vector<uint32> mIndexBuffer;

    for (auto& [mesh, commands] : mCommands)
    {
        const VulkanMesh* vulkanMesh = reinterpret_cast<VulkanMesh*>(mesh);
        vulkanMesh->Bind(mCommandBuffers[mNextImageIndex]);
        for (const auto& command : commands)
        {
            const ScarlettMath::Mat4 scale = ScarlettMath::ScaleMatrix(command.transform->scale);
            ScarlettMath::Mat4 translation = ScarlettMath::TranslateMatrix(command.transform->translation);

            const ModelInfo info
            {
                .view = camera->GetViewMatrix(),
                .proj = camera->GetProjectionMatrix(),
                .model = translation * command.transform->rotation.GetRotationMatrix() * scale,

                .color = command.color,
                .material = command.material
            };

            vkCmdBindDescriptorSets(mCommandBuffers[mNextImageIndex], VK_PIPELINE_BIND_POINT_GRAPHICS, mPipelineLayout, 0, 1, &mDescriptorSet, 0, nullptr);
            vkCmdPushConstants(mCommandBuffers[mNextImageIndex], mPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(ModelInfo), &info);

            vulkanMesh->Draw(mCommandBuffers[mNextImageIndex]);
        }
        commands.clear();
    }

    vkCmdEndRenderPass(mCommandBuffers[mNextImageIndex]);

#ifndef SCARLETT_EDITOR_ENABLED
    VK_CHECK(vkEndCommandBuffer(mCommandBuffers[mNextImageIndex]), "Failed to end recording Vulkan Command Buffer.");

    VK_CHECK(mSwapChain->SubmitCommandBuffers(&mCommandBuffers[mNextImageIndex], &mNextImageIndex), "Failed to present Vulkan Swap Chain Image");
#endif // SCARLETT_EDITOR_ENABLED.
}

void VulkanRenderer::OnWindowResize(const uint32 width, const uint32 height)
{
    if (width == 0 || height == 0) { return; }

    RecreateSwapChain(width, height);

    CreatePipeline();
}

void VulkanRenderer::CreatePipelineLayout()
{
    constexpr VkPushConstantRange pushConstantRange
    {
        .stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
        .offset = 0,
        .size = sizeof(ModelInfo)
    };

    vector<VkDescriptorSetLayout> setLayouts = {
    //existingSetLayout,   // maybe for uniforms, materials, etc.
    mDescriptorSetLayout     // your big texture array descriptor set
    };

    // ReSharper disable once CppVariableCanBeMadeConstexpr
    const VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .pNext                      = VK_NULL_HANDLE,
        .flags                      = 0,
        .setLayoutCount             = 1,
        .pSetLayouts                = &mDescriptorSetLayout,
        .pushConstantRangeCount     = 1,
        .pPushConstantRanges        = &pushConstantRange,
    };

    VK_CHECK(vkCreatePipelineLayout(mDevice.GetDevice(), &pipelineLayoutCreateInfo, nullptr, &mPipelineLayout), "Failed to create Vulkan Pipeline Layout.");
}

void VulkanRenderer::CreatePipeline()
{
    PipelineConfigInfo pipelineConfig{};
    Pipeline::DefaultPipelineConfigInfo(pipelineConfig, mSwapChain->GetWidth(), mSwapChain->GetHeight());

    pipelineConfig.renderPass       = mSwapChain->GetRenderPass();
    pipelineConfig.pipelineLayout   = mPipelineLayout;

    if(mPipeline)
    {
        mPipeline->Destroy();
        delete mPipeline;
    }
    mPipeline = new Pipeline();

    mPipeline->Init(&mDevice, Filepath{ "ScarlettEngine/ScarlettEngine/Rendering/Shaders/vert.spv" }, Filepath{ "ScarlettEngine/ScarlettEngine/Rendering/Shaders/frag.spv" }, pipelineConfig);
}

void VulkanRenderer::CreateCommandBuffers()
{
    mCommandBuffers.resize(mSwapChain->GetImageCount());

    const VkCommandBufferAllocateInfo commandBufferAllocInfo
    {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .pNext                  = VK_NULL_HANDLE,
        .commandPool            = mDevice.GetCommandPool(),
        .level                  = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount     = static_cast<uint32>(mCommandBuffers.size()),
    };

    VK_CHECK(vkAllocateCommandBuffers(mDevice.mDevice, &commandBufferAllocInfo, mCommandBuffers.data()), "Failed to allocate Vulkan Command Buffer");
}

void VulkanRenderer::RecreateSwapChain(const uint32 width, const uint32 height)
{
    vkDeviceWaitIdle(mDevice.GetDevice());

    const bool previousSwapChainExists = mSwapChain;

    mSwapChain = new SwapChain(&mDevice, { width, height }, mSwapChain);

    if(mSwapChain->GetImageCount() != mCommandBuffers.size() && previousSwapChainExists)
    {
        FreeCommandBuffers();
        CreateCommandBuffers();
    }
}

void VulkanRenderer::RecordCommandBuffer(const uint32 imageIndex) const
{
    mPipeline->Bind(mCommandBuffers[imageIndex]);
}

void VulkanRenderer::FreeCommandBuffers()
{
    vkFreeCommandBuffers(mDevice.GetDevice(), mDevice.GetCommandPool(), static_cast<uint32>(mCommandBuffers.size()), mCommandBuffers.data());
    mCommandBuffers.clear();
}

} // Namespace Scarlett.
