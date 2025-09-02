#pragma once

#include "Rendering/Mesh.h"

namespace Scarlett
{

class Device;

class MeshManager
{
public:
    MeshManager();
    ~MeshManager();

    MeshManager(const MeshManager&)               = delete;
    MeshManager(MeshManager&&)                    = delete;
    MeshManager& operator=(MeshManager&&)         = delete;
    MeshManager& operator=(const MeshManager&)    = delete;

    //todo change this to be a meshId instead of a weak ref, this way we can fix the reinterpret cast
    [[nodiscard]] WeakRef<Mesh> AddMesh(const Filepath& filepath);
private:
    WeakRef<Device> mRendererDevice;

    vector<Mesh*>   mLoadedMeshes;
};

} // Namespace Scarlett.
