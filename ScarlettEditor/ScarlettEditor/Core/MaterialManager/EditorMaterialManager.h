#pragma once

#include "src/ScarlettEditor.h"

#include <Core/FileIO/Filepath.h>

#include <Rendering/MaterialInfo.h>
#include <Resources/Manager/MaterialManager.h>

namespace ScarlettEditor
{
/**
 * @brief Editor version of \c MaterialManager. Does what the engine version does, with the addition of some editor only methods/information.
 */
class EDITOR_API EditorMaterialManager final : public Scarlett::MaterialManager
{
public:
    EditorMaterialManager()             = default;
    ~EditorMaterialManager() override   = default;

    EditorMaterialManager(const EditorMaterialManager&)               = delete;
    EditorMaterialManager(EditorMaterialManager&&)                    = delete;
    EditorMaterialManager& operator=(EditorMaterialManager&&)         = delete;
    EditorMaterialManager& operator=(const EditorMaterialManager&)    = delete;

    void EditorCreateMaterial(const ScarlettUtils::Ulid ulid, const std::string& materialFriendlyName, const Scarlett::Filepath& albedo);

    [[nodiscard]] Scarlett::MaterialInfo GetMaterialCreateInfo(const ScarlettUtils::Ulid ulid) const override;
    [[nodiscard]] const unordered_map<ScarlettUtils::Ulid, Scarlett::MaterialInfo>& GetProcessedMaterialMap() const override { return mProcessedMaterialMap; }
};

} // Namespace ScarlettEditor.