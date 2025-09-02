#pragma once

#include <Math/Math.h>

#include <vulkan/vulkan.h>

#include "Rendering/Mesh.h"

#include "Resources/Types/Vertex.h"

namespace Scarlett
{

class Device;
class VertexBuffer;
class IndexBuffer;

struct VulkanVertex
{
    Resource::Vertex vertex;

    static vector<VkVertexInputBindingDescription>      GetBindingDescriptions();
    static vector<VkVertexInputAttributeDescription>    GetAttributeDescriptions();
};

struct Model
{
    alignas(16) ScarlettMath::Vec3 position;
    alignas(16) ScarlettMath::Vec3 color;
};

class VulkanMesh final : public Mesh
{
public:
    //VulkanMesh() = default;
    VulkanMesh(const WeakRef<Device> device, const Filepath& filepath);
    ~VulkanMesh() override;

    VulkanMesh(const VulkanMesh&)               = delete;
    VulkanMesh(VulkanMesh&&)                    = delete;
    VulkanMesh& operator=(VulkanMesh&&)         = delete;
    VulkanMesh& operator=(const VulkanMesh&)    = delete;

    void Bind(const VkCommandBuffer commandBuffer) const;
    void Draw(const VkCommandBuffer commandBuffer, const uint32 instanceCount = 1) const;
private:
    const VertexBuffer*   mVertexBuffer;
    const IndexBuffer*    mIndexBuffer;
};

} // Namespace Scarlett.
