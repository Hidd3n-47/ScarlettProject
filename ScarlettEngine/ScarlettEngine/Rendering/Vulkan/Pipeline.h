#pragma once

#include <vulkan/vulkan_core.h>

namespace Scarlett {

class Device;

struct PipelineConfigInfo
{
    PipelineConfigInfo()  = default;
    ~PipelineConfigInfo() = default;

    PipelineConfigInfo(const PipelineConfigInfo&)               = delete;
    PipelineConfigInfo(PipelineConfigInfo&&)                    = delete;
    PipelineConfigInfo& operator=(PipelineConfigInfo&&)         = delete;
    PipelineConfigInfo& operator=(const PipelineConfigInfo&)    = delete;

    VkRect2D    scissor;
    VkViewport  viewport;

    VkPipelineInputAssemblyStateCreateInfo  inputAssemblyCreateInfo{};
    VkPipelineRasterizationStateCreateInfo  rasterizationCreateInfo{};
    VkPipelineMultisampleStateCreateInfo    multiSampleCreateInfo{};
    VkPipelineColorBlendAttachmentState     colorBlendAttachment{};
    VkPipelineColorBlendStateCreateInfo     colorBlendCreateInfo{};
    VkPipelineDepthStencilStateCreateInfo   depthStencilCreateInfo{};

    VkPipelineLayout    pipelineLayout  = nullptr;
    VkRenderPass        renderPass      = nullptr;
    uint32              subPass         = 0;
};

class Pipeline
{
public:
    Pipeline()  = default;
    ~Pipeline() = default;

    Pipeline(const Pipeline&)               = delete;
    Pipeline(Pipeline&&)                    = delete;
    Pipeline& operator=(Pipeline&&)         = delete;
    Pipeline& operator=(const Pipeline&)    = delete;

    void Init(Device* device, const Filepath& vertFilepath, const Filepath& fragFilepath, const PipelineConfigInfo& configInfo);
    void Destroy();

    void Bind(const VkCommandBuffer commandBuffer) const;

    [[nodiscard]] VkPipeline GetGraphicsPipeline() const { return mGraphicsPipeline; }

    static void DefaultPipelineConfigInfo(PipelineConfigInfo& configInfo, const uint32 width, const uint32 height);
private:
    Device*         mDevice;
    VkPipeline      mGraphicsPipeline;

    // Todo christian make this not return a copy vector.
    static vector<char> ReadFile(const Filepath& filepath);

    void CreateGraphicsPipeline(const Filepath& vertFilepath, const Filepath& fragFilepath, const PipelineConfigInfo& configInfo);

    void CreateShaderModule(const vector<char>& code, VkShaderModule* shaderModule) const;
};

} // Namespace Scarlett.