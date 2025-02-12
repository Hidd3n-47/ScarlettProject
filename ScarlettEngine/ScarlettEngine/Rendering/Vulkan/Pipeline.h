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

    VkPipelineInputAssemblyStateCreateInfo  inputAssemblyCreateInfo;
    VkPipelineRasterizationStateCreateInfo  rasterizationCreateInfo;
    VkPipelineMultisampleStateCreateInfo    multiSampleCreateInfo;
    VkPipelineColorBlendAttachmentState     colorBlendAttachment;
    VkPipelineColorBlendStateCreateInfo     colorBlendCreateInfo;
    VkPipelineDepthStencilStateCreateInfo   depthStencilCreateInfo;

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

    void Init(Device* device, const std::string& vertFilepath, const std::string& fragFilepath, const PipelineConfigInfo& configInfo);
    void Destroy();

    void Bind(VkCommandBuffer commandBuffer) const;

    [[nodiscard]] VkPipeline GetGraphicsPipeline() const { return mGraphicsPipeline; }

    static void DefaultPipelineConfigInfo(PipelineConfigInfo& configInfo, const uint32 width, const uint32 height);
private:
    static vector<char> ReadFile(const std::string& filepath);

    void CreateGraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath, const PipelineConfigInfo& configInfo);

    void CreateShaderModule(const vector<char>& code, VkShaderModule* shaderModule) const;

    Device*         mDevice;
    VkPipeline      mGraphicsPipeline;
    VkShaderModule  mVertexShaderModule;
    VkShaderModule  mFragmentShaderModule;
};

} // Namespace Scarlett.