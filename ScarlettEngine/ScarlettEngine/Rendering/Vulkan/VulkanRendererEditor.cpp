#include "ScarlettEnginePch.h"
#include "VulkanRendererEditor.h"

#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_vulkan.h>

#include "VulkanUtils.h"
#include "Core/Window/Window.h"

namespace Scarlett
{

#ifdef SCARLETT_EDITOR_ENABLED

static ImVec4 HexToRgba(int a, int b, int c)
{
    return { a / 255.0f, b / 255.0f, c / 255.0f, 1.0f };
}

static ImVec4 Dark      = HexToRgba(0x17, 0x17, 0x17); // #171717FF
static ImVec4 Medium    = HexToRgba(0x2A, 0x2A, 0x2A); // #2A2A2AFF
static ImVec4 Light     = HexToRgba(0xA9, 0xA9, 0xA9);
static ImVec4 OffWhite  = HexToRgba(0xF5, 0xF5, 0xF5);

void VulkanRendererEditor::Init(const Window* windowRef)
{
    VulkanRenderer::Init(windowRef);

    mSwapChain = new SwapChain(&mDevice, { windowRef->GetWidth(), windowRef->GetHeight() }, mSwapChain);

 /*   if (mSwapChain->GetImageCount() != mCommandBuffers.size() && previousSwapChainExists)
    {
        FreeCommandBuffers();
        CreateCommandBuffers();
    }*/

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

    VkDescriptorPoolCreateInfo pool_info = {};
    pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    pool_info.flags             = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    pool_info.maxSets           = 1000;
    pool_info.poolSizeCount     = std::size(poolSizes);
    pool_info.pPoolSizes        = poolSizes;

    vkCreateDescriptorPool(mDevice.mDevice, &pool_info, nullptr, &mImGuiPool);

    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui_ImplGlfw_InitForVulkan(static_cast<GLFWwindow*>(mWindowRef->GetNativeWindow()), true);

    ImGui_ImplVulkan_InitInfo initInfo = {};
    initInfo.Instance               = mDevice.mInstance;
    initInfo.PhysicalDevice         = mDevice.mPhysicalDevice;
    initInfo.Device                 = mDevice.mDevice;
    initInfo.QueueFamily            = mDevice.mQueueFamilyIndices.graphicsFamily;
    initInfo.Queue                  = mDevice.mGraphicsQueue;
    initInfo.PipelineCache          = VK_NULL_HANDLE;
    initInfo.DescriptorPool         = mImGuiPool;
    initInfo.Subpass                = 0;
    initInfo.MinImageCount          = 2;
    initInfo.ImageCount             = 2;
    initInfo.MSAASamples            = VK_SAMPLE_COUNT_1_BIT;
    initInfo.Allocator              = nullptr;
    initInfo.CheckVkResultFn        = nullptr;

#ifdef SCARLETT_EDITOR_ENABLED
    initInfo.RenderPass = mSwapChain->GetEditorRenderPass();
#else
    initInfo.RenderPass = mSwapChain->GetRenderPass();
#endif // SCARLETT_EDITOR_ENABLED.


    ImGui::StyleColorsDark();
    io.Fonts->AddFontDefault();

    ImGui_ImplVulkan_Init(&initInfo);

    ImGui_ImplVulkan_CreateFontsTexture();

    ImGuiStyle& style = ImGui::GetStyle();
    style.Alpha             = 1.0f;
    style.WindowRounding    = 0.0f;
    style.FrameRounding     = 0.0f;

    // style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
    // style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.00f, 0.40f, 0.41f, 1.00f);
     style.Colors[ImGuiCol_WindowBg] = Dark;
    // style.Colors[ImGuiCol_Border] = ImVec4(0.00f, 1.00f, 1.00f, 0.65f);
    // style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    // style.Colors[ImGuiCol_FrameBg] = ImVec4(0.44f, 0.80f, 0.80f, 0.18f);
    // style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.44f, 0.80f, 0.80f, 0.27f);
    // style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.44f, 0.81f, 0.86f, 0.66f);
    // style.Colors[ImGuiCol_TitleBg] = ImVec4(0.14f, 0.18f, 0.21f, 0.73f);
    // style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.54f);
    // style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 1.00f, 1.00f, 0.27f);
    // style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.20f);
    // style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.22f, 0.29f, 0.30f, 0.71f);
    // style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.00f, 1.00f, 1.00f, 0.44f);
    // style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.00f, 1.00f, 1.00f, 0.74f);
    // style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
    // style.Colors[ImGuiCol_CheckMark] = ImVec4(0.00f, 1.00f, 1.00f, 0.68f);
    // style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.00f, 1.00f, 1.00f, 0.36f);
    // style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.00f, 1.00f, 1.00f, 0.76f);
    // style.Colors[ImGuiCol_Button] = ImVec4(0.00f, 0.65f, 0.65f, 0.46f);
    // style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.01f, 1.00f, 1.00f, 0.43f);
    // style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.00f, 1.00f, 1.00f, 0.62f);
    // style.Colors[ImGuiCol_Header] = ImVec4(0.00f, 1.00f, 1.00f, 0.33f);
    // style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 1.00f, 1.00f, 0.42f);
    // style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.00f, 1.00f, 1.00f, 0.54f);
    // style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 1.00f, 1.00f, 0.54f);
    // style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.00f, 1.00f, 1.00f, 0.74f);
    // style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
    // style.Colors[ImGuiCol_PlotLines] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
    // style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
    // style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
    // style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
    // style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 1.00f, 1.00f, 0.22f);
    // style.Colors[ImGuiCol_TabActive] = ImVec4(0.00f, 0.60f, 0.61f, 0.80f);
    // style.Colors[ImGuiCol_Tab] = ImVec4(0.00f, 0.40f, 0.41f, 0.40f);
    // style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.00f, 0.40f, 0.41f, 1.00f);

    VkSamplerCreateInfo samplerCreateInfo = {};
    samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerCreateInfo.magFilter = VK_FILTER_LINEAR;
    samplerCreateInfo.minFilter = VK_FILTER_LINEAR;
    samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerCreateInfo.anisotropyEnable = VK_FALSE;
    samplerCreateInfo.maxAnisotropy = 1.0f;
    samplerCreateInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;
    samplerCreateInfo.compareEnable = VK_FALSE;
    samplerCreateInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

    vkCreateSampler(mDevice.mDevice, &samplerCreateInfo, nullptr, &sampler);
}

void VulkanRendererEditor::Destroy()
{
    vkDeviceWaitIdle(mDevice.mDevice);

    vkDestroySampler(mDevice.mDevice, sampler, nullptr);

    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    vkDestroyDescriptorPool(mDevice.mDevice, mImGuiPool, nullptr);

    VulkanRenderer::Destroy();
}

void VulkanRendererEditor::BeginRender()
{
    VulkanRenderer::BeginRender();
}

void VulkanRendererEditor::Render()
{
    VulkanRenderer::Render();
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
    ImGui::DockSpaceOverViewport();

    bool show = true;
    ImGui::ShowDemoWindow(&show);

    RenderPropertiesPanel();

    ImGui::Begin("Console");

    ImGui::End();


    ImGui::Begin("Scene");

    ImGui::End();

    ImGui::Begin("Viewport");

    static int num = 0;
    num++;


    textureID[textureIndex] = ImGui_ImplVulkan_AddTexture(sampler, mSwapChain->GetViewportImageView(mNextImageIndex), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    const ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    ImGui::Image((ImTextureID)textureID[textureIndex], viewportPanelSize);

    textureIndex = (textureIndex + 1) % 3;
    if (textureID[textureIndex])
    {
        ImGui_ImplVulkan_RemoveTexture(textureID[textureIndex]);
    }

    ImGui::End();
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

void VulkanRendererEditor::RenderPropertiesPanel()
{
    //ImGui::PushStyleColor(ImGuiCol_WindowBg, Medium);
    ImGui::Begin("Properties");

    if (ImGui::CollapsingHeader("Background Color"))
    {
        ImGui::ColorPicker3("##Background Color Picker", mClearColor);
    }

    ImGui::End();
    //ImGui::PopStyleColor();
}

#endif // SCARLETT_EDITOR_ENABLED.

} // Namespace Scarlett.
