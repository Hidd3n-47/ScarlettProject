#pragma once

#include "Vertex.h"

namespace Scarlett::Resource
{

struct Mesh
{
    vector<Vertex> vertices;
    vector<uint32> indices;
};

} // Namespace Scarlett::Resource.
