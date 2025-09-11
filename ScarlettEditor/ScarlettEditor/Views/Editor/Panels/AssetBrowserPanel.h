#pragma once

#include <filesystem>

#include "UI/Panel.h"
#include "Core/AssetManagement/AssetTypes.h"

namespace ScarlettEditor
{

class DirectoryCache;

class AssetBrowserPanel final : public Panel
{
public:
    //explicit inline AssetBrowserPanel(IView* view) : Panel{ view, {.title = "Asset Browser" } } { /* Empty */ }
    explicit AssetBrowserPanel(IView* view);

    void Render() override;

private:
    bool mCreateFile    = false;
    bool mCreateFolder  = false;
    bool mContextOpen   = false;

    AssetTypes mAssetTypeToCreate = AssetTypes::TEXT_FILE;

    std::filesystem::path   mSelectedDirectory;

    DirectoryCache*         mDirectory;

    void RenderContextMenu() override;

    bool DrawDirectoryTree(const std::filesystem::path& directoryPath, const uint32 indentAmount = 0);

    void CreateAssetFromSelectedType(const std::filesystem::path& path) const;
};

} // Namespace ScarlettEditor.
