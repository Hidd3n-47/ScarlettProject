#pragma once

#include "Rendering/Vulkan/VulkanRenderer.h"

namespace Scarlett
{

class VulkanRendererEditor final : VulkanRenderer
{
public:
    VulkanRendererEditor() = default;
    ~VulkanRendererEditor() override = default;

    VulkanRendererEditor(const VulkanRendererEditor&)               = delete;
    VulkanRendererEditor(VulkanRendererEditor&&)                    = delete;
    VulkanRendererEditor& operator=(VulkanRendererEditor&&)         = delete;
    VulkanRendererEditor& operator=(const VulkanRendererEditor&)    = delete;

    void Init(const Window* windowRef) override;
    void Destroy() override;

    void BeginRender() override;
    void Render() override;
    void EndRender() override;
private:
    VkDescriptorPool mImGuiPool;
};

} // Namespace Scarlett.