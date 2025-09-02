#include "ScarlettEditorPch.h"
#include "AssetManager.h"

#include "Core/DirectoryCache.h"

#include <ScarlEntt/Types.h>

#include <ScarlettUtils/Xml/XmlSerializer.h>

namespace ScarlettEditor
{

AssetManager::AssetManager()
    : mDirectoryCache{ new DirectoryCache{ ASSET_PATH } }
{
    // Empty.
    RecursivelyProcessDirectory(mDirectoryCache);
}

AssetManager::~AssetManager()
{
    delete mDirectoryCache;
}

void AssetManager::RefreshDirectoryCache()
{
    delete mDirectoryCache;
    mDirectoryCache = new DirectoryCache{ ASSET_PATH };
}

void AssetManager::RecursivelyProcessDirectory(const DirectoryCache* cache)
{
    static constexpr std::string_view ASSET_TYPE_ATTRIBUTE_NAME { "AssetType" };
    static constexpr std::string_view MATERIAL_ATTRIBUTE_NAME   { "Material" };

    for (const auto& child : cache->GetDirectoryChildren())
    {
        for (const auto& grandchild : child.GetDirectoryChildren())
        {
            if (grandchild.IsDirectory())
            {
                RecursivelyProcessDirectory(&grandchild);
            }
            else
            {
                if (grandchild.GetExtension() == SCARLETT_ASSET_EXTENSION)
                {
                    const ScarlettUtils::XmlDocument doc = ScarlettUtils::XmlSerializer::Deserialize(grandchild.GetPath().string());

                    /*if (const std::string attributeValue = doc.GetRootElement()->GetAttributeValue(std::string{ ASSET_TYPE_ATTRIBUTE_NAME }); attributeValue == MATERIAL_ATTRIBUTE_NAME)
                    {
                        EDITOR_DLOG("Creating material with name: {0}", grandchild.GetPath().filename().string().c_str());
                    }
                    else
                    {
                        EDITOR_ELOG("Failed to deduce what type of asset type ['{0}'] at path: {1}", attributeValue, grandchild.GetPath().filename().string().c_str());
                    }*/
                }
            }
        }
    }
}

} // Namespace ScarlettEditor.