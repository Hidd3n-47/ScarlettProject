#pragma once

#include "Vertex.h"

namespace Scarlett::Resource
{

struct MeshData
{
    vector<Vertex> vertices;
    vector<uint32> indices;
};

} // Namespace Scarlett::Resource.
