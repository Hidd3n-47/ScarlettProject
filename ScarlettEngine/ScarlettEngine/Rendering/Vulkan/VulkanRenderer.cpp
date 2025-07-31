#include "ScarlettEnginepch.h"
#include "VulkanRenderer.h"

#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

#include <ScarlettGameCore/Src/GameCore.h>

#include <Components/Camera.h>

#include "VulkanUtils.h"
#include "VulkanMesh.h"
#include "VulkanTexture.h"

#include "Core/Window/Window.h"
#include "Rendering/SpriteInfoStruct.h"

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
        const VkDescriptorPoolSize poolSizes[] =
        {
            { VK_DESCRIPTOR_TYPE_SAMPLER,                   1000 },
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,    1000 },
            { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,             1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,             1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,      1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER,      1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,            1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,            1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,    1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC,    1000 },
            { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,          1000 }
        };

        VkDescriptorPoolCreateInfo imGuiDescriptorPoolInfo = {};
        imGuiDescriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        imGuiDescriptorPoolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        imGuiDescriptorPoolInfo.maxSets = 1000;
        imGuiDescriptorPoolInfo.poolSizeCount = std::size(poolSizes);
        imGuiDescriptorPoolInfo.pPoolSizes = poolSizes;

        vkCreateDescriptorPool(mDevice.mDevice, &imGuiDescriptorPoolInfo, nullptr, &mDescriptorSetPool);
        // -----------------------------------------------------------------------------------

        mTexture = new VulkanTexture();
        mTexture->SetDevice(&mDevice);
        //constexpr std::array<uint8, 4> color = { 0xff, 0xff, 0xff, 0xff };
        //mTexture->Create(color.data(), 1, 1);
        mTexture->Create(Filepath{"Assets/TextureUV.png"});

        constexpr VkDescriptorSetLayoutBinding textureDescriptorSet
        {
            .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            .descriptorCount    = 1,
            .stageFlags         = VK_SHADER_STAGE_FRAGMENT_BIT,
            //.pImmutableSamplers = sampler
        };

        const VkDescriptorSetLayoutCreateInfo textureDescriptorSetInfo
        {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
            .bindingCount   = 1,
            .pBindings      = &textureDescriptorSet
        };
        VK_CHECK(vkCreateDescriptorSetLayout(mDevice.GetDevice(), &textureDescriptorSetInfo, nullptr, &mTextureDescriptorSetLayout), "Failed to create Vulkan Descriptor Set for Texture.");

        const VkDescriptorSetAllocateInfo descriptorSetAllocateInfo
        {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
            .descriptorPool     = mDescriptorSetPool,
            .descriptorSetCount = 1,
            .pSetLayouts        = &mTextureDescriptorSetLayout
        };

        vkAllocateDescriptorSets(mDevice.GetDevice(), &descriptorSetAllocateInfo, &mTextureDescriptorSet);

        const VkWriteDescriptorSet wds
        {
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .dstSet             = mTextureDescriptorSet,
            .dstBinding         = 0,
            .descriptorCount    = 1,
            .descriptorType     = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            .pImageInfo         = &mTexture->GetImageInfo()
        };

        vkUpdateDescriptorSets(mDevice.GetDevice(), 1, &wds, 0, nullptr);

        CreatePipelineLayout();

        RecreateSwapChain(mWindowRef->GetWidth(), mWindowRef->GetHeight());
        CreatePipeline();

        CreateCommandBuffers();

        mSquare = new VulkanMesh(&mDevice, Filepath{ "Assets/Mesh/Plane.obj" });
        mLine = new VulkanMesh(&mDevice, Filepath{ "Assets/Mesh/CylinderLowPoly.obj" });
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
    vkDestroyDescriptorSetLayout(mDevice.GetDevice(), mTextureDescriptorSetLayout, nullptr);

    delete mLine;
    delete mSquare;

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
    for (size_t i{ 0 }; i < mCommands[RenderType::SPRITE].size(); ++i)
    {
        const RenderCommand command = mCommands[RenderType::SPRITE][i];

        const ScarlettMath::Mat4 scale = ScarlettMath::ScaleMatrix(command.transform->scale);
        ScarlettMath::Mat4 translation = ScarlettMath::TranslateMatrix(command.transform->translation);

        const SpriteInfoStruct info
        {
            .color = command.color,
            .view  = camera->GetViewMatrix(),
            .proj  = camera->GetProjectionMatrix(),
            .model = translation * command.transform->rotation.GetRotationMatrix() * scale
        };

        vkCmdBindDescriptorSets(mCommandBuffers[mNextImageIndex], VK_PIPELINE_BIND_POINT_GRAPHICS, mPipelineLayout, 0, 1, &mTextureDescriptorSet, 0, nullptr);
        vkCmdPushConstants(mCommandBuffers[mNextImageIndex], mPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SpriteInfoStruct), &info);

        mSquare->Bind(mCommandBuffers[mNextImageIndex]);
        mSquare->Draw(mCommandBuffers[mNextImageIndex]);
    }
    mCommands[RenderType::SPRITE].clear();

    //Lines.
    for (size_t i{ 0 }; i < mCommands[RenderType::LINE].size(); ++i)
    {
        const RenderCommand command = mCommands[RenderType::LINE][i];

        const ScarlettMath::Mat4 scale = ScarlettMath::ScaleMatrix(command.transform->scale);
        ScarlettMath::Mat4 translation = ScarlettMath::TranslateMatrix(command.transform->translation);

        const SpriteInfoStruct info
        {
            .color = command.color,
            .view  = camera->GetViewMatrix(),
            .proj  = camera->GetProjectionMatrix(),
            .model = translation * command.transform->rotation.GetRotationMatrix() * scale
        };

        vkCmdBindDescriptorSets(mCommandBuffers[mNextImageIndex], VK_PIPELINE_BIND_POINT_GRAPHICS, mPipelineLayout, 0, 1, &mTextureDescriptorSet, 0, nullptr);
        vkCmdPushConstants(mCommandBuffers[mNextImageIndex], mPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SpriteInfoStruct), &info);

        mLine->Bind(mCommandBuffers[mNextImageIndex]);
        mLine->Draw(mCommandBuffers[mNextImageIndex]);
    }
    mCommands[RenderType::LINE].clear();

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
        .size = sizeof(SpriteInfoStruct)
    };

    // ReSharper disable once CppVariableCanBeMadeConstexpr
    const VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .pNext                      = VK_NULL_HANDLE,
        .flags                      = 0,
        .setLayoutCount             = 1,
        .pSetLayouts                = &mTextureDescriptorSetLayout,
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
