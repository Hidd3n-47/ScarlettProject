#include "ScarlettEnginepch.h"
#include "Pipeline.h"

#include "Device.h"
#include "Mesh.h"
#include "Rendering/Vulkan/VulkanUtils.h"

namespace Scarlett
{

void Pipeline::Init(Device* device, const std::string& vertFilepath, const std::string& fragFilepath, const PipelineConfigInfo& configInfo)
{
    mDevice = device;

    CreateGraphicsPipeline(vertFilepath, fragFilepath, configInfo);
}

void Pipeline::Destroy()
{
    vkDestroyPipeline(mDevice->GetDevice(), mGraphicsPipeline, nullptr);
    mGraphicsPipeline = nullptr;
}

void Pipeline::Bind(const VkCommandBuffer commandBuffer) const
{
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, mGraphicsPipeline);
}

void Pipeline::DefaultPipelineConfigInfo(PipelineConfigInfo& configInfo, const uint32 width, const uint32 height)
{
    configInfo.scissor = {};
    configInfo.scissor.offset = { .x = 0, .y = 0};
    configInfo.scissor.extent = { .width = width, .height = height };

    configInfo.viewport = {};
    configInfo.viewport.x           = 0.0f;
    configInfo.viewport.y           = 0.0f;
    configInfo.viewport.width       = static_cast<float>(width);
    configInfo.viewport.height      = static_cast<float>(height);
    configInfo.viewport.minDepth    = 0.0f;
    configInfo.viewport.maxDepth    = 1.0f;

    configInfo.inputAssemblyCreateInfo = {};
    configInfo.inputAssemblyCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    configInfo.inputAssemblyCreateInfo.topology                 = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    configInfo.inputAssemblyCreateInfo.primitiveRestartEnable   = VK_FALSE;
    // todo Christian Remove the two comments below once we get there.
    configInfo.rasterizationCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    configInfo.rasterizationCreateInfo.depthClampEnable             = VK_FALSE;
    configInfo.rasterizationCreateInfo.rasterizerDiscardEnable      = VK_FALSE;
    configInfo.rasterizationCreateInfo.polygonMode                  = VK_POLYGON_MODE_FILL;
    configInfo.rasterizationCreateInfo.lineWidth                    = 1.0f;
    configInfo.rasterizationCreateInfo.cullMode                     = VK_CULL_MODE_NONE; // VK_CULL_MODE_BACK_BIT
    configInfo.rasterizationCreateInfo.frontFace                    = VK_FRONT_FACE_COUNTER_CLOCKWISE; // clockwise?
    configInfo.rasterizationCreateInfo.depthBiasEnable              = VK_FALSE;

    configInfo.multiSampleCreateInfo = {};
    configInfo.multiSampleCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    configInfo.multiSampleCreateInfo.sampleShadingEnable    = VK_FALSE;
    configInfo.multiSampleCreateInfo.rasterizationSamples   = VK_SAMPLE_COUNT_1_BIT;

    configInfo.colorBlendAttachment = {};
    configInfo.colorBlendAttachment.colorWriteMask          = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    configInfo.colorBlendAttachment.blendEnable             = VK_TRUE;
    configInfo.colorBlendAttachment.srcColorBlendFactor     = VK_BLEND_FACTOR_SRC_ALPHA;
    configInfo.colorBlendAttachment.dstColorBlendFactor     = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    configInfo.colorBlendAttachment.colorBlendOp            = VK_BLEND_OP_ADD;
    configInfo.colorBlendAttachment.srcAlphaBlendFactor     = VK_BLEND_FACTOR_ONE;
    configInfo.colorBlendAttachment.dstAlphaBlendFactor     = VK_BLEND_FACTOR_ZERO;
    configInfo.colorBlendAttachment.alphaBlendOp            = VK_BLEND_OP_ADD;

    configInfo.colorBlendCreateInfo = {};
    configInfo.colorBlendCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    configInfo.colorBlendCreateInfo.logicOpEnable       = VK_FALSE;
    configInfo.colorBlendCreateInfo.logicOp             = VK_LOGIC_OP_COPY;
    configInfo.colorBlendCreateInfo.attachmentCount     = 1;
    configInfo.colorBlendCreateInfo.pAttachments        = &configInfo.colorBlendAttachment;
    configInfo.colorBlendCreateInfo.blendConstants[0]   = 0.0f;
    configInfo.colorBlendCreateInfo.blendConstants[1]   = 0.0f;
    configInfo.colorBlendCreateInfo.blendConstants[2]   = 0.0f;
    configInfo.colorBlendCreateInfo.blendConstants[3]   = 0.0f;

    configInfo.depthStencilCreateInfo = {};
    configInfo.depthStencilCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    configInfo.depthStencilCreateInfo.depthTestEnable           = VK_TRUE;
    configInfo.depthStencilCreateInfo.depthWriteEnable          = VK_TRUE;
    configInfo.depthStencilCreateInfo.depthCompareOp            = VK_COMPARE_OP_LESS;
    configInfo.depthStencilCreateInfo.depthBoundsTestEnable     = VK_FALSE;
    configInfo.depthStencilCreateInfo.stencilTestEnable         = VK_FALSE;
}

vector<char> Pipeline::ReadFile(const std::string& filepath)
{
    std::ifstream fin{ filepath, std::ios::binary | std::ios::ate };

    if (!fin.is_open())
    {
        SCARLETT_ELOG("Failed to open file at path: {0}", filepath);
        throw std::runtime_error("Failed to open file at path: " + filepath);
    }

    size_t fileSize = fin.tellg();
    vector<char> fileBuffer(fileSize);

    fin.seekg(0);

    fin.read(fileBuffer.data(), static_cast<uint32>(fileSize));

    fin.close();

    return fileBuffer;
}

void Pipeline::CreateGraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath, const PipelineConfigInfo& configInfo)
{
    const vector<char> vert = ReadFile(vertFilepath);
    const vector<char> frag = ReadFile(fragFilepath);

    CreateShaderModule(vert, &mVertexShaderModule);
    CreateShaderModule(frag, &mFragmentShaderModule);

    VkPipelineShaderStageCreateInfo shaderStages[2]
    {
        // Vertex Shader.
        {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .flags  = 0,
            .stage  = VK_SHADER_STAGE_VERTEX_BIT,
            .module = mVertexShaderModule,
            .pName  = "main"
        },
        // Fragment Shader.
        {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .flags  = 0,
            .stage  = VK_SHADER_STAGE_FRAGMENT_BIT,
            .module = mFragmentShaderModule,
            .pName  = "main"
        }
    };

    auto bindingDescriptions = Vertex::GetBindingDescriptions();
    auto attributeDescriptions = Vertex::GetAttributeDescriptions();
    VkPipelineVertexInputStateCreateInfo vertexInputInfo
    { 
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        .vertexBindingDescriptionCount      = static_cast<uint32>(bindingDescriptions.size()),
        .pVertexBindingDescriptions         = bindingDescriptions.data(),
        .vertexAttributeDescriptionCount    = static_cast<uint32>(attributeDescriptions.size()),
        .pVertexAttributeDescriptions       = attributeDescriptions.data()
    };

    VkPipelineViewportStateCreateInfo viewportCreateInfo
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        .viewportCount      = 1,
        .pViewports         = &configInfo.viewport,
        .scissorCount       = 1,
        .pScissors          = &configInfo.scissor
    };

    const VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo
    {
        .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .stageCount             = 2,
        .pStages                = shaderStages,
        .pVertexInputState      = &vertexInputInfo,
        .pInputAssemblyState    = &configInfo.inputAssemblyCreateInfo,
        .pViewportState         = &viewportCreateInfo,
        .pRasterizationState    = &configInfo.rasterizationCreateInfo,
        .pMultisampleState      = &configInfo.multiSampleCreateInfo,
        .pDepthStencilState     = &configInfo.depthStencilCreateInfo,
        .pColorBlendState       = &configInfo.colorBlendCreateInfo,
        .pDynamicState          = nullptr,
        .layout                 = configInfo.pipelineLayout,
        .renderPass             = configInfo.renderPass,
        .subpass                = configInfo.subPass,
        .basePipelineHandle     = VK_NULL_HANDLE,
        .basePipelineIndex      = -1,
    };

    VK_CHECK(vkCreateGraphicsPipelines(mDevice->GetDevice(), VK_NULL_HANDLE, 1, &graphicsPipelineCreateInfo, nullptr, &mGraphicsPipeline), "Failed to create a vulkan graphics pipeline.");

    // Todo Christian Can I not destroy the shader modules here?
    // Todo Christian remove from being member variables.
    vkDestroyShaderModule(mDevice->GetDevice(), mFragmentShaderModule, nullptr);
    vkDestroyShaderModule(mDevice->GetDevice(), mVertexShaderModule, nullptr);
}

void Pipeline::CreateShaderModule(const vector<char>& code, VkShaderModule* shaderModule) const
{
    VkShaderModuleCreateInfo createInfo
    {
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .codeSize   = code.size(),
        .pCode      = reinterpret_cast<const uint32*>(code.data())
    };

    VK_CHECK(vkCreateShaderModule(mDevice->GetDevice(), &createInfo, nullptr, shaderModule), "Failed to create vulkan shader module.");
}

} // Namespace Scarlett.