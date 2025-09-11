#include "ScarlettEnginePch.h"
#include "VulkanRendererEditor.h"

#ifdef SCARLETT_EDITOR_ENABLED

#include <ScarlettGameCore/Src/GameCore.h>

#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_vulkan.h>

#include <ScarlettEditor/Editor/EditorManager.h>

#include "Core/Engine.h"
#include "VulkanUtils.h"
#include "Core/Window/Window.h"

namespace Scarlett
{

std::unique_ptr<Renderer> Renderer::mInstance = std::make_unique<VulkanRendererEditor>();

void VulkanRendererEditor::Init(const Window* windowRef)
{
    VulkanRenderer::Init(windowRef);

    mSwapChain = new SwapChain(&mDevice, { windowRef->GetWidth(), windowRef->GetHeight() }, mSwapChain);

 /*   if (mSwapChain->GetImageCount() != mCommandBuffers.size() && previousSwapChainExists)
    {
        FreeCommandBuffers();
        CreateCommandBuffers();
    }*/

    // Todo this is a part of ImGui set up so should somehow be restricted only for editor mode.
    // -----------------------------------------------------------------------------------
    constexpr uint32 MAX_COUNT = 1000;
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

    const VkDescriptorPoolCreateInfo imGuiDescriptorPoolInfo
    {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        .flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
        .maxSets = MAX_COUNT,
        .poolSizeCount = std::size(poolSizes),
        .pPoolSizes = poolSizes
    };

    vkCreateDescriptorPool(mDevice.mDevice, &imGuiDescriptorPoolInfo, nullptr, &mImGuiDescriptorSetPool);
    // -----------------------------------------------------------------------------------

    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui_ImplGlfw_InitForVulkan(static_cast<GLFWwindow*>(mWindowRef->GetNativeWindow()), false);

    ImGui_ImplVulkan_InitInfo initInfo = {};
    initInfo.Instance               = mDevice.mInstance;
    initInfo.PhysicalDevice         = mDevice.mPhysicalDevice;
    initInfo.Device                 = mDevice.mDevice;
    initInfo.QueueFamily            = mDevice.mQueueFamilyIndices.graphicsFamily;
    initInfo.Queue                  = mDevice.mGraphicsQueue;
    initInfo.PipelineCache          = VK_NULL_HANDLE;
    initInfo.DescriptorPool         = mImGuiDescriptorSetPool;
    initInfo.Subpass                = 0;
    initInfo.MinImageCount          = 2;
    initInfo.ImageCount             = 2;
    initInfo.MSAASamples            = VK_SAMPLE_COUNT_1_BIT;
    initInfo.Allocator              = nullptr;
    initInfo.CheckVkResultFn        = nullptr;
    initInfo.RenderPass             = mSwapChain->GetEditorRenderPass();

    io.FontDefault = io.Fonts->AddFontFromFileTTF("E:/Programming/ScarlettProject/Assets/Fonts/Roboto/Roboto-Medium.ttf", 15);

    ImGui_ImplVulkan_Init(&initInfo);
    ImGui_ImplVulkan_CreateFontsTexture();

    constexpr VkSamplerCreateInfo samplerCreateInfo
    {
        .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
        .magFilter                 = VK_FILTER_LINEAR,
        .minFilter                 = VK_FILTER_LINEAR,
        .mipmapMode                = VK_SAMPLER_MIPMAP_MODE_LINEAR,
        .addressModeU              = VK_SAMPLER_ADDRESS_MODE_REPEAT,
        .addressModeV              = VK_SAMPLER_ADDRESS_MODE_REPEAT,
        .addressModeW              = VK_SAMPLER_ADDRESS_MODE_REPEAT,
        .anisotropyEnable          = VK_FALSE,
        .maxAnisotropy             = 1.0f,
        .compareEnable             = VK_FALSE,
        .compareOp                 = VK_COMPARE_OP_ALWAYS,
        .borderColor               = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
        .unnormalizedCoordinates   = VK_FALSE,
    };

    vkCreateSampler(mDevice.mDevice, &samplerCreateInfo, nullptr, &mSampler);

    // ImGui does not support cross-boundary access between lib and dll, therefore retrieve them to pass to the Editor.
    ImGuiMemAllocFunc allocateFunction;
    ImGuiMemFreeFunc freeFunction;
    void* data;
    ImGui::GetAllocatorFunctions(&allocateFunction, &freeFunction, &data);
    ScarlettEditor::EditorManager::CreateInstance(ImGui::GetCurrentContext(), &allocateFunction, &freeFunction, data);
    ScarlettEditor::EditorManager::Instance().Init(Engine::Instance().GetMaterialManager().Cast<ScarlettEditor::EditorMaterialManager>());
}

void VulkanRendererEditor::Destroy()
{
    vkDeviceWaitIdle(mDevice.mDevice);

    ScarlettEditor::EditorManager::DestroyInstance();

    vkDestroySampler(mDevice.mDevice, mSampler, nullptr);

    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    vkDestroyDescriptorPool(mDevice.mDevice, mImGuiDescriptorSetPool, nullptr);

    VulkanRenderer::Destroy();
}

void VulkanRendererEditor::BeginRender()
{
    VulkanRenderer::BeginRender();
}

void VulkanRendererEditor::EndRender()
{
    VulkanRenderer::EndRender();

    BeginRenderEditor();
    RenderEditor();
    EndRenderEditor();
}

void VulkanRendererEditor::BeginRenderEditor() const
{
    const VkRect2D renderArea
    {
        .offset = { 0, 0 },
        .extent = mSwapChain->GetSwapChainExtent()
    };

    std::array<VkClearValue, 1> clearValues;
    clearValues[0].color = { { 0.1f, 0.1f, 0.1f, 1.0f } };

    const VkRenderPassBeginInfo renderPassInfoEditor
    {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        .renderPass         = mSwapChain->GetEditorRenderPass(),
        .framebuffer        = mSwapChain->GetEditorFrameBuffer(static_cast<int>(mNextImageIndex)),
        .renderArea         = renderArea,
        .clearValueCount    = static_cast<uint32>(clearValues.size()),
        .pClearValues       = clearValues.data()
    };

    vkCmdBeginRenderPass(mCommandBuffers[mNextImageIndex], &renderPassInfoEditor, VK_SUBPASS_CONTENTS_INLINE);

    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void VulkanRendererEditor::RenderEditor()
{
    mViewportTexture[mCurrentTextureIndex] = ImGui_ImplVulkan_AddTexture(mSampler, mSwapChain->GetViewportImageView(mNextImageIndex), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    ScarlettEditor::EditorManager::Instance().RenderUi((ImTextureID)mViewportTexture[mCurrentTextureIndex]);

    mCurrentTextureIndex = (mCurrentTextureIndex + 1) % 3;
    if (mViewportTexture[mCurrentTextureIndex])
    {
        ImGui_ImplVulkan_RemoveTexture(mViewportTexture[mCurrentTextureIndex]);
    }
}

void VulkanRendererEditor::EndRenderEditor() const
{
    ImGui::Render();
    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), mCommandBuffers[mNextImageIndex]);

    ImGui::EndFrame();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();

    vkCmdEndRenderPass(mCommandBuffers[mNextImageIndex]);

    VK_CHECK(vkEndCommandBuffer(mCommandBuffers[mNextImageIndex]), "Failed to end recording Vulkan Command Buffer.");

    VK_CHECK(mSwapChain->SubmitCommandBuffers(&mCommandBuffers[mNextImageIndex], &mNextImageIndex), "Failed to present Vulkan Swap Chain Image");
}

} // Namespace Scarlett.

#endif // SCARLETT_EDITOR_ENABLED.
