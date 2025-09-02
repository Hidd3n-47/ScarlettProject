#pragma once

namespace ScarlettEditor
{

class DirectoryCache;

class AssetManager
{
public:
    AssetManager();
    ~AssetManager();

    AssetManager(const AssetManager&)               = delete;
    AssetManager(AssetManager&&)                    = delete;
    AssetManager& operator=(AssetManager&&)         = delete;
    AssetManager& operator=(const AssetManager&)    = delete;

    void RefreshDirectoryCache();

    static constexpr std::string_view ASSET_PATH{ "E:/Programming/ScarlettProject/Assets/" };
    static constexpr std::string_view SCARLETT_ASSET_EXTENSION{ ".scarlett_asset" };
private:
    DirectoryCache* mDirectoryCache;

    static void RecursivelyProcessDirectory(const DirectoryCache* cache);
};

} // Namespace ScarlettEditor.