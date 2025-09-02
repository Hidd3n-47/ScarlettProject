#pragma once

#include "Resources/Types/MeshData.h"

namespace Scarlett
{

class Mesh
{
public:
    Mesh()          = default;
    virtual ~Mesh() = default;

    Mesh(const Mesh&)               = delete;
    Mesh(Mesh&&)                    = delete;
    Mesh& operator=(Mesh&&)         = delete;
    Mesh& operator=(const Mesh&)    = delete;
protected:
    Resource::MeshData    mMesh;
};

} // Namespace Scarlett.
