#include "ScarlettEditorPch.h"
#include "EditorMaterialManager.h"

#include <Rendering/MaterialInfo.h>

namespace ScarlettEditor
{

void EditorMaterialManager::EditorCreateMaterial(const ScarlettUtils::Ulid ulid, const std::string& materialFriendlyName, const Scarlett::Filepath& albedo)
{
    // If the maps contain the ULID, this means the material has already been loaded (loaded map) or processed from the XML (material map).
    if (mLoadedMaterialMap.contains(ulid) || mMaterialMapToProcess.contains(ulid))
    {
        const uint64 ulidRaw = ulid;
        EDITOR_ELOG("Material with ULID: '{0}' has already been added.", ulidRaw);
    }

    mMaterialMapToProcess[ulid] = Scarlett::MaterialInfo{ ulid, materialFriendlyName, albedo };
}

Scarlett::MaterialInfo EditorMaterialManager::GetMaterialCreateInfo(const ScarlettUtils::Ulid ulid) const
{
    if (!mLoadedMaterialMap.contains(ulid))
    {
        const uint64 ulidRaw = ulid;
        EDITOR_ELOG("Requested material info with invalid ULID: '{0}'", ulidRaw);
    }

    return mProcessedMaterialMap.at(ulid);
}

} // Namespace ScarlettEditor.