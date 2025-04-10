#include "ScarlettEnginepch.h"
#include "VulkanRenderer.h"

#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

#include <ScarlettGameCore/Components/Camera.h>

#include "Mesh.h"
#include "VulkanUtils.h"
#include "Core/Window/Window.h"
#include "Rendering/SpriteInfoStruct.h"
#include "Rendering/Commands/SpriteCommand.h"

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
        CreatePipelineLayout();

        RecreateSwapChain(mWindowRef->GetWidth(), mWindowRef->GetHeight());
        CreatePipeline();

        CreateCommandBuffers();

        const vector<Vertex> verts
        {
                {{ -0.5f, -0.5f} },
                {{ -0.5f,  0.5f} },
                {{  0.5f,  0.5f} },
                {{  0.5f, -0.5f} },
            };
        const vector<uint32> indices
        {
            0, 1, 2,
            2, 3, 0
        };
        mSquare = new Mesh(&mDevice, verts, indices);
    }
    catch(const std::runtime_error& e)
    {
        SCARLETT_FLOG("Error: {0}", e.what());
    }
}

void VulkanRenderer::Destroy()
{
    vkDeviceWaitIdle(mDevice.mDevice);

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
    const ScarlettGame::Camera* camera = GetRenderCamera();

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
            .view  = camera->viewMatrix,
            .proj  = camera->projectionMatrix,
            .model = translation * command.transform->rotation.GetRotationMatrix() * scale
        };

        vkCmdPushConstants(mCommandBuffers[mNextImageIndex], mPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SpriteInfoStruct), &info);

        mSquare->Bind(mCommandBuffers[mNextImageIndex]);
        mSquare->Draw(mCommandBuffers[mNextImageIndex]);
    }
    mCommands[RenderType::SPRITE].clear();

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
        .setLayoutCount             = 0,
        .pSetLayouts                = nullptr,
        .pushConstantRangeCount     = 1,
        .pPushConstantRanges        = &pushConstantRange,
    };

    VK_CHECK(vkCreatePipelineLayout(mDevice.GetDevice(), &pipelineLayoutCreateInfo, nullptr, &mPipelineLayout), "Failed to create Vulkan Pipeline Layout.");
}

void VulkanRenderer::CreatePipeline()
{
    PipelineConfigInfo pipelineConfig{};
    Pipeline::DefaultPipelineConfigInfo(pipelineConfig, mSwapChain->GetWidth(), mSwapChain->GetHeight());

    pipelineConfig.renderPass = mSwapChain->GetRenderPass();
    pipelineConfig.pipelineLayout = mPipelineLayout;

    if(mPipeline)
    {
        mPipeline->Destroy();
        delete mPipeline;
    }
    mPipeline = new Pipeline();

    mPipeline->Init(&mDevice, "E:/Programming/ScarlettProject/ScarlettEngine/ScarlettEngine/Rendering/Shaders/vert.spv", "E:/Programming/ScarlettProject/ScarlettEngine/ScarlettEngine/Rendering/Shaders/frag.spv", pipelineConfig);
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
