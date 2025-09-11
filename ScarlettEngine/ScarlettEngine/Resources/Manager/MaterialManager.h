#pragma once

#include <Components/Material.h>

#include <ScarlettUtils/Core/Ulid.h>

namespace Scarlett
{

class Device;
struct MaterialInfo;

/**
 * @class MaterialManager: The \c MaterialManager class is used to handle materials.
 * The \c MaterialManager will be responsible for loading and creating the materials.
 * In dev configuration the material manager retains some extra information such as all the processed materials so that the editor can have some extra functionality such
 * as seeing which materials have been loaded.
 * The main responsibility for \c MaterialManager is to manage materials, not textures. Materials can contain textures, however, the \c TextureManager is responsible for loading
 * and creating textures. The \c MaterialManager is just to organise the mapping of a material \c Ulid to the indices/information needed on the GPU.
 */
class MaterialManager
{
public:
    MaterialManager()           = default;
    virtual ~MaterialManager()  = default;

    MaterialManager(const MaterialManager&)               = delete;
    MaterialManager(MaterialManager&&)                    = delete;
    MaterialManager& operator=(MaterialManager&&)         = delete;
    MaterialManager& operator=(const MaterialManager&)    = delete;

    /**
     * @brief A function used to initialize the \c MaterialManager. This will set up default textures that the engine needs.
     */
    void Init();

    /**
     * @brief A function used to generate the materials that have yet to be processed.
     */
    void UpdateMaterials();

    /**
     * @brief A function used to create a texture for the given ulid.
     * @param materialUlid The \c Ulid of the material that is being created.
     * @param materialCreateInfo The \MaterialInfo that will be used to create the material.
     */
    void CreateMaterial(const ScarlettUtils::Ulid materialUlid, const MaterialInfo& materialCreateInfo);

    /**
     * @brief Get the information of the material that needs to be uploaded to the GPU.
     * @param material The material whose upload information is being requested for.
     * @return The \c MaterialUploadInfo of the material. If no information exist, will get invalid material information.
     */
    [[nodiscard]] MaterialUploadInfo GetMaterialUploadInfo(const Material& material) const;

#ifdef DEV_CONFIGURATION
    /**
     * @brief Get the \c MaterialInfo for a material with the passed in \c Ulid.
     * @param ulid The \c Ulid of the material whose information is being requested.
     * @return The \c MaterialInfo for material with the passed in \c Ulid.
     */
    [[nodiscard]] virtual MaterialInfo GetMaterialCreateInfo(const ScarlettUtils::Ulid ulid)        const = 0;
    /**
     * @brief Get the material map of all the materials that have been processed. This map can be used to map the material \c Ulid to its \c MaterialInfo, i.e. information relating to the XML/creation info.
     * @return The unordered map between \c Ulid and \c Material info of all materials that have been processed and created.
     */
    [[nodiscard]] virtual const unordered_map<ScarlettUtils::Ulid, MaterialInfo>& GetProcessedMaterialMap()  const = 0;
#endif // DEV_CONFIGURATION.

    static constexpr ScarlettUtils::Ulid DEFAULT_MATERIAL_ULID { 0 };
    static constexpr ScarlettUtils::Ulid INVALID_MATERIAL_ULID { 1 };
protected:
    /**
     * @brief This map is for materials that need processing. Add to this map if the material has not been created and needs to be created before the next \c CommandBuffer is started.
     * This map will be cleared after the materials have been processed and created.
     */
    unordered_map<ScarlettUtils::Ulid, MaterialInfo>        mMaterialMapToProcess;
    /**
     * @brief This map is a map of all the materials that have been processed and created.
     * This map will not be cleared for the lifetime of the manager, as this will retain all materials that have been processed.
     */
    unordered_map<ScarlettUtils::Ulid, MaterialInfo>        mProcessedMaterialMap;
    /**
     * @brief Similar to the \c mProcessedMaterialMap, this contains materials that have been processed. This map is a mapping to the information that is uploaded to the GPU.
     * The difference between this map and \c mProcessedMaterialMap is the fact that this map is for GPU, whilst \c mProcessedMaterialMap is the info used to create this material (i.e. the info on disk).
     */
    unordered_map<ScarlettUtils::Ulid, MaterialUploadInfo>  mLoadedMaterialMap;
};


} // Namespace Scarlett.
