#pragma once

#include "Rendering/Renderer.h"
#include "Utilities.h"

#include "Device.h"
#include "Pipeline.h"
#include "SwapChain.h"

namespace Scarlett
{

class SquareSprite;

class VulkanRenderer : Renderer
{
    friend class VulkanRendererEditor;
public:
    VulkanRenderer() = default;
    virtual ~VulkanRenderer() override = default;

    VulkanRenderer(const VulkanRenderer&)               = delete;
    VulkanRenderer(VulkanRenderer&&)                    = delete;
    VulkanRenderer& operator=(VulkanRenderer&&)         = delete;
    VulkanRenderer& operator=(const VulkanRenderer&)    = delete;

    void Init(const Window* windowRef) override;
    void Destroy() override;

    void BeginRender() override;
    void Render() override;
    void EndRender() override;

    void OnWindowResize(const uint32 width, const uint32 height) override;
private:
    void CreatePipelineLayout();
    void CreatePipeline();
    void CreateCommandBuffers();

    void RecreateSwapChain(const uint32 width, const uint32 height);
    void RecordCommandBuffer(const uint32 imageIndex) const;
    void FreeCommandBuffers();

    Device      mDevice;
    SwapChain*  mSwapChain;
    Pipeline*   mPipeline = nullptr;

    VkPipelineLayout            mPipelineLayout;
    vector<VkCommandBuffer>     mCommandBuffers;

    uint32 mNextImageIndex;

    // Temp.
    SquareSprite* mSquare;
    SquareSprite* mSquare2;
};

} // Namespace Scarlett.