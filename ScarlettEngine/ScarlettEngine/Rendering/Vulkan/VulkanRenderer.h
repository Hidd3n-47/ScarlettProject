#pragma once

#include "Rendering/Renderer.h"
#include "Utilities.h"

#include "Device.h"
#include "Pipeline.h"
#include "SwapChain.h"

namespace Scarlett
{

class Mesh;

class VulkanRenderer : public Renderer
{
    friend class VulkanRendererEditor;
public:
    VulkanRenderer() = default;
    ~VulkanRenderer() override = default;

    VulkanRenderer(const VulkanRenderer&)               = delete;
    VulkanRenderer(VulkanRenderer&&)                    = delete;
    VulkanRenderer& operator=(VulkanRenderer&&)         = delete;
    VulkanRenderer& operator=(const VulkanRenderer&)    = delete;

    // todo add camera for release
    inline ScarlettGame::Camera*    GetRenderCamera() override { return nullptr; }

    void Init(const Window* windowRef) override;
    void Destroy() override;

    void BeginRender()  override;
    void Render()       override;
    void EndRender()    override;

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

    Mesh* mSquare;
    Mesh* mLine;

    // Temp.
    float mClearColor[3] = { 0.1f, 0.1f, 0.1f };
};

} // Namespace Scarlett.
