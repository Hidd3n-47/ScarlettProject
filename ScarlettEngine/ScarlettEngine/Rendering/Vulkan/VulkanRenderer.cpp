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

    Engine::Instance().GetTextureManager()->Destroy();

    FreeCommandBuffers();

    mPipeline->Destroy();
    delete mPipeline;
    mPipeline = nullptr;
    vkDestroyPipelineLayout(mDevice.GetDevice(), mPipelineLayout, nullptr);

    SCARLETT_ASSERT(mSwapChain && "Trying to destroy a swap chain that was not created.");
    delete mSwapChain;
    mSwapChain= nullptr;

    mDevice.Destroy();
}

void VulkanRenderer::BeginRender()
{
    // First thing we need to do is create the materials if needed.
    Engine::Instance().GetMaterialManager()->UpdateMaterials();

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

            const auto& materialInfo = Engine::Instance().GetMaterialManager()->GetMaterialUploadInfo(command.material);

            const ModelInfo info
            {
                .view       = camera->GetViewMatrix(),
                .proj       = camera->GetProjectionMatrix(),
                .model      = translation * command.transform->rotation.GetRotationMatrix() * scale,

                .color      = command.color,
                .material   = materialInfo
            };

            vkCmdBindDescriptorSets(mCommandBuffers[mNextImageIndex], VK_PIPELINE_BIND_POINT_GRAPHICS, mPipelineLayout, 0, 1, &Engine::Instance().GetTextureManager()->mDescriptorSet, 0, nullptr);
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

    const vector<VkDescriptorSetLayout> setLayouts =
    {
        //existingSetLayout,   // maybe for uniforms, materials, etc.
        Engine::Instance().GetTextureManager()->mDescriptorSetLayout
    };

    const VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .pNext                      = VK_NULL_HANDLE,
        .flags                      = 0,
        .setLayoutCount             = 1,
        .pSetLayouts                = setLayouts.data(),
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

    mPipeline->Init(&mDevice, Filepath{ "EngineAssets/Shaders/vert.spv" }, Filepath{ "EngineAssets/Shaders/frag.spv" }, pipelineConfig);
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
