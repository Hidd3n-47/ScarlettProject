#pragma once

#include <Math/Math.h>

#include "Utilities.h"

#include "VertexBuffer.h"

namespace Scarlett
{

class Device;

struct Vertex
{
    ScarlettMath::Vec2 position;

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
    Mesh(Device* device, const vector<Vertex>& vertices);
    ~Mesh();

    Mesh(const Mesh&)               = delete;
    Mesh(Mesh&&)                    = delete;
    Mesh& operator=(Mesh&&)         = delete;
    Mesh& operator=(const Mesh&)    = delete;

    void Bind(const VkCommandBuffer commandBuffer) const;
    void Draw(const VkCommandBuffer commandBuffer) const;
private:
    // TODO Remove device.
    Device*         mDevice;
    VertexBuffer*   mVertexBuffer;
    uint32          mVertexCount;
};

} // Namespace Scarlett.
