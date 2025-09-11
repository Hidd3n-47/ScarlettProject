#pragma once

#include <ScarlettUtils/Core/Ulid.h>

namespace Scarlett
{

/**
 * @struct MaterialUploadInfo: This struct holds the information of the material that should be uploaded to the GPU.
 * Since \c Material just holds the \c Ulid of the material, this will be the processed information that will be uploaded to the GPU.
 */
struct MaterialUploadInfo
{
    textureId albedoId;
};

/**
 * @class Material: Stores an \c Ulid to the material that should be used during the render pipeline.
 * The material is similar to a component, hence why it is just the \c Ulid, however, it technically is not a component, but rather used by components that render things.
 */
class Material
{
public:
    Material() = default;
    explicit Material(const ScarlettUtils::Ulid ulid) : mUlid(ulid) { /* Empty. */ }

    [[nodiscard]] inline ScarlettUtils::Ulid GetUlid() const { return mUlid; }
    inline void SetUlid(const ScarlettUtils::Ulid ulid) { mUlid = ulid; }
private:
    ScarlettUtils::Ulid mUlid;
};

} // Namespace Scarlett.
