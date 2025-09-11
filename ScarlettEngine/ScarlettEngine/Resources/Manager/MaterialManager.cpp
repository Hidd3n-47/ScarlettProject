#include "ScarlettEnginePch.h"
#include "MaterialManager.h"

#include "Core/Engine.h"
#include "TextureManager.h"

#include "Rendering/MaterialInfo.h"

namespace Scarlett
{

void MaterialManager::Init()
{
    // Create the map to the default and invalid ULIDs. These materials won't be loaded from disk as they are engine defaults and hence need to be manually added.
    mProcessedMaterialMap[DEFAULT_MATERIAL_ULID] = MaterialInfo{ DEFAULT_MATERIAL_ULID, "Mat_Default", Filepath{ "" } };
    mProcessedMaterialMap[INVALID_MATERIAL_ULID] = MaterialInfo{ INVALID_MATERIAL_ULID, "Mat_Invalid", Filepath{ "" } };

    mLoadedMaterialMap[DEFAULT_MATERIAL_ULID] = MaterialUploadInfo{ TextureManager::WHITE_TEXTURE_INDEX };
    mLoadedMaterialMap[INVALID_MATERIAL_ULID] = MaterialUploadInfo{ TextureManager::INVALID_TEXTURE_INDEX };
}

void MaterialManager::UpdateMaterials()
{
    for (const auto& [ulid, materialCreateInfo] : mMaterialMapToProcess)
    {
        CreateMaterial(ulid, materialCreateInfo);
    }

    mMaterialMapToProcess.clear();
}

void MaterialManager::CreateMaterial(const ScarlettUtils::Ulid materialUlid, const MaterialInfo& materialCreateInfo)
{
    MaterialUploadInfo matInfo;

    if (mLoadedMaterialMap.contains(materialUlid))
    {
        const uint64 ulidRaw = materialUlid;
        SCARLETT_WLOG("Trying to create a material for a processed material ULID: '{0}'", ulidRaw);
        matInfo = mLoadedMaterialMap[materialUlid];
    }
    else
    {
        matInfo.albedoId = Engine::Instance().GetTextureManager()->AddTexture(materialCreateInfo.albedoTexture);
    }

    mProcessedMaterialMap[materialUlid] = materialCreateInfo;
    mLoadedMaterialMap[materialUlid] = matInfo;
}

MaterialUploadInfo MaterialManager::GetMaterialUploadInfo(const Material& material) const
{
    if (mLoadedMaterialMap.contains(material.GetUlid()))
    {
        return mLoadedMaterialMap.at(material.GetUlid());
    }

    const uint64 ulidRaw = material.GetUlid();
    SCARLETT_WLOG("Trying to access material upload info for a material that has not been created yet. ULID: '{0}'", ulidRaw);

    return mLoadedMaterialMap.at(INVALID_MATERIAL_ULID);;

}
} // Namespace Scarlett.
