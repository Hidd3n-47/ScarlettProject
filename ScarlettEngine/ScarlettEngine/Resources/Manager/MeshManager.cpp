#include "ScarlettEnginePch.h"
#include "MeshManager.h"

#include "Rendering/Vulkan/VulkanMesh.h"
#include "Rendering/Vulkan/VulkanRenderer.h"

namespace Scarlett
{

MeshManager::MeshManager()
{
    VulkanRenderer* renderer = dynamic_cast<VulkanRenderer*>(Renderer::Instance());
    SCARLETT_ASSERT(renderer && "Renderer failed to be cast to vulkan renderer. Please check graphics API and add functionality to Mesh Manager.");

    mRendererDevice = renderer->GetDevice();
}

MeshManager::~MeshManager()
{
    for (const Mesh* mesh : mLoadedMeshes)
    {
        delete mesh;
    }
}

WeakRef<Mesh> MeshManager::AddMesh(const Filepath& filepath)
{
    mLoadedMeshes.emplace_back(new VulkanMesh{ mRendererDevice, filepath });

    return WeakRef{ mLoadedMeshes.back() };
}


} // Namespace Scarlett.
