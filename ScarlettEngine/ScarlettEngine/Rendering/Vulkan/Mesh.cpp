#include "ScarlettEnginepch.h"
#include "Mesh.h"

#include "Utilities.h"
#include "Device.h"

namespace Scarlett
{

vector<VkVertexInputBindingDescription> Vertex::GetBindingDescriptions()
{
    vector< VkVertexInputBindingDescription> bindingDescriptions(1);

    bindingDescriptions[0].binding      = 0;
    bindingDescriptions[0].stride       = sizeof(Vertex);
    bindingDescriptions[0].inputRate    = VK_VERTEX_INPUT_RATE_VERTEX;

    return bindingDescriptions;
}

vector<VkVertexInputAttributeDescription> Vertex::GetAttributeDescriptions()
{
    vector< VkVertexInputAttributeDescription> bindingAttributes(1);

    bindingAttributes[0].binding        = 0;
    bindingAttributes[0].location       = 0;
    bindingAttributes[0].format         = VK_FORMAT_R32G32_SFLOAT;
    bindingAttributes[0].offset         = offsetof(Vertex, position);

    //bindingAttributes[1].binding        = 0;
    //bindingAttributes[1].location       = 1;
    //bindingAttributes[1].format         = VK_FORMAT_R32G32B32A32_SFLOAT;
    //bindingAttributes[1].offset         = offsetof(Vertex, color);

    return bindingAttributes;
}

Mesh::Mesh(Device* device, const vector<Vertex>& vertices)
    : mDevice(device)
    , mVertexBuffer { new VertexBuffer{ device, sizeof(vertices), vertices.data()} }
    , mVertexCount { static_cast<uint32>(vertices.size()) }
{
    // Empty.
}

Mesh::~Mesh()
{
    delete mVertexBuffer;
}

void Mesh::Bind(const VkCommandBuffer commandBuffer) const
{
    mVertexBuffer->Bind(commandBuffer);
}


void Mesh::Draw(const VkCommandBuffer commandBuffer) const
{
    vkCmdDraw(commandBuffer, mVertexCount, 1, 0, 0);
}

} // Namespace Scarlett.