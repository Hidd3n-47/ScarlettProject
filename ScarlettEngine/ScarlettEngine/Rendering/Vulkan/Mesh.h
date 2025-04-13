#pragma once

#include <Math/Math.h>

#include "Utilities.h"

namespace Scarlett
{

class Device;
class VertexBuffer;
class IndexBuffer;

struct Vertex
{
    ScarlettMath::Vec3 position;

    static vector<VkVertexInputBindingDescription> GetBindingDescriptions();
    static vector<VkVertexInputAttributeDescription> GetAttributeDescriptions();
};

struct Model
{
    alignas(16) ScarlettMath::Vec3 position;
    alignas(16) ScarlettMath::Vec3 color;
};

class Mesh
{
public:
    Mesh() = default;
    Mesh(Device* device, const vector<Vertex>& vertices, const vector<uint32_t>& indices);
    Mesh(Device* device, const std::string& filepath);
    ~Mesh();

    Mesh(const Mesh&)               = delete;
    Mesh(Mesh&&)                    = delete;
    Mesh& operator=(Mesh&&)         = delete;
    Mesh& operator=(const Mesh&)    = delete;

    void Bind(const VkCommandBuffer commandBuffer) const;
    void Draw(const VkCommandBuffer commandBuffer) const;
private:
    const VertexBuffer*   mVertexBuffer;
    const IndexBuffer*    mIndexBuffer;
    const vector<uint32>  mIndices;
};

} // Namespace Scarlett.
