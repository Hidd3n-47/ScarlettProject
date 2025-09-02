#pragma once

#include <filesystem>

#include "UI/Panel.h"

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

    void RenderContextMenu() override;

    bool DrawDirectoryTree(const std::filesystem::path& directoryPath, const uint32 indentAmount = 0);

    std::filesystem::path mSelectedDirectory;
    bool mContextOpen = false;

    DirectoryCache* mDirectory;
};

} // Namespace ScarlettEditor.
