#pragma once

#include "Rendering/Vulkan/VulkanRenderer.h"

namespace Scarlett
{

#ifdef SCARLETT_EDITOR_ENABLED

class VulkanRendererEditor final : public VulkanRenderer
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

    void BeginRenderEditor() const;
    void RenderEditor();
    void EndRenderEditor() const;
private:
    VkDescriptorPool        mImGuiPool;
    VkSampler               mSampler;
    VkDescriptorSet         mViewportTexture[3];
    uint32                  mCurrentTextureIndex;

    void RenderPropertiesPanel();
};

#endif // SCARLETT_EDITOR_ENABLED.

} // Namespace Scarlett.
