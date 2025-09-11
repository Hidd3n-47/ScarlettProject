#pragma once

#include <ScarlettUtils/Core/Ulid.h>

namespace Scarlett
{
/**
 * @struct MaterialInfo: This struct holds information relating to the material, mainly for the editor life-span so that we don't have to keep processing
 * the XML on disk, but rather the information cached in memory.
 * @note This information should be updated if the material on disk changes any information.
 */
struct MaterialInfo
{
    MaterialInfo() = default;
    inline MaterialInfo(const ScarlettUtils::Ulid ulid, std::string friendlyName, Filepath path)
        : ulid(ulid)
        , friendlyName(std::move(friendlyName))
        , albedoTexture(std::move(path))
    { /* Empty. */ }

    ScarlettUtils::Ulid ulid{};
    std::string         friendlyName;
    Filepath            albedoTexture{"" };
};

} // Namespace Scarlett.