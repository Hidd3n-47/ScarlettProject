#include "ScarlettEnginepch.h"
#include "VulkanMesh.h"

#include "Device.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"

#include "Resources/Loading/MeshLoader.h"

namespace Scarlett
{

vector<VkVertexInputBindingDescription> VulkanVertex::GetBindingDescriptions()
{
    vector< VkVertexInputBindingDescription> bindingDescriptions(1);

    bindingDescriptions[0].binding      = 0;
    bindingDescriptions[0].stride       = sizeof(Resource::Vertex);
    bindingDescriptions[0].inputRate    = VK_VERTEX_INPUT_RATE_VERTEX;

    return bindingDescriptions;
}

vector<VkVertexInputAttributeDescription> VulkanVertex::GetAttributeDescriptions()
{
#ifdef DEV_CONFIGURATION
    static_assert(sizeof(VulkanVertex) == sizeof(Resource::Vertex) && "Vulkan Vertex and Vertex should be the same size to ensure attributes align.");
#endif // DEV_CONFIGURATION.

    vector<VkVertexInputAttributeDescription> bindingAttributes(3);

    bindingAttributes[0].binding        = 0;
    bindingAttributes[0].location       = 0;
    bindingAttributes[0].format         = VK_FORMAT_R32G32B32_SFLOAT;
    bindingAttributes[0].offset         = offsetof(Resource::Vertex, position);

    bindingAttributes[1].binding        = 0;
    bindingAttributes[1].location       = 1;
    bindingAttributes[1].format         = VK_FORMAT_R32G32B32_SFLOAT;
    bindingAttributes[1].offset         = offsetof(Resource::Vertex, normal);

    bindingAttributes[2].binding        = 0;
    bindingAttributes[2].location       = 2;
    bindingAttributes[2].format         = VK_FORMAT_R32G32_SFLOAT;
    bindingAttributes[2].offset         = offsetof(Resource::Vertex, textureCoord);

    return bindingAttributes;
}

VulkanMesh::VulkanMesh(Device* device, const Filepath& filepath)
{
    MeshLoader::LoadMesh(filepath, mMesh);

    mVertexBuffer  = new VertexBuffer{ device, sizeof(Resource::Vertex) * mMesh.vertices.size(), mMesh.vertices.data() };
    mIndexBuffer   = new IndexBuffer{ device, static_cast<uint32>(mMesh.indices.size()), mMesh.indices.data() };
}

VulkanMesh::~VulkanMesh()
{
    delete mIndexBuffer;
    delete mVertexBuffer;
}

void VulkanMesh::Bind(const VkCommandBuffer commandBuffer) const
{
    mVertexBuffer->Bind(commandBuffer);
    mIndexBuffer->Bind(commandBuffer);
}

void VulkanMesh::Draw(const VkCommandBuffer commandBuffer) const
{
    vkCmdDrawIndexed(commandBuffer, mIndexBuffer->GetNumberOfIndices(), 1, 0, 0, 0);
}

} // Namespace Scarlett.
