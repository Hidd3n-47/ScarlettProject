#include "ScarlettEditorPch.h"
#include "AssetManager.h"

#include <ScarlEntt/Types.h>
#include <ScarlettUtils/Xml/XmlSerializer.h>

#include "Core/DirectoryCache.h"

#include "Serialization/MaterialDeserializer.h"

namespace ScarlettEditor
{

namespace
{
const std::string TEXT_FILE_STRING      { "Text File" };
const std::string MATERIAL_FILE_STRING  { "Material" };
const std::string UNKNOWN_FILE_STRING   { "UnknownAssetType" };
} // Anonymous Namespace.

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

std::string AssetManager::AssetTypeToString(const AssetTypes type)
{
    switch (type)
    {
    case AssetTypes::TEXT_FILE:
        return TEXT_FILE_STRING;
    case AssetTypes::MATERIAL_FILE:
        return MATERIAL_FILE_STRING;
    }

    return UNKNOWN_FILE_STRING;
}

AssetTypes AssetManager::StringToAssetType(const std::string& typeString)
{
    if (typeString == TEXT_FILE_STRING)
    {
        return AssetTypes::TEXT_FILE;
    }
    if (typeString == MATERIAL_FILE_STRING)
    {
        return AssetTypes::MATERIAL_FILE;
    }

    // If the type is unrecognised, treat as a text document.
    return AssetTypes::TEXT_FILE;
}

void AssetManager::RecursivelyProcessDirectory(const DirectoryCache* cache)
{
    static constexpr std::string_view ASSET_TYPE_ATTRIBUTE_NAME { "AssetType" };
    const std::string materialAttributeName{ AssetTypeToString(AssetTypes::MATERIAL_FILE) };

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
                    if (!doc.IsValidDocument())
                    {
                        EDITOR_ELOG("Failed to deserialize document at path: {0}", grandchild.GetPath().filename().string().c_str());
                        return;
                    }

                    if (const std::string attributeValue = doc.GetRootElement()->GetAttributeValue(std::string{ ASSET_TYPE_ATTRIBUTE_NAME }); attributeValue == materialAttributeName)
                    {
                        EDITOR_DLOG("Creating material with name: {0}", grandchild.GetPath().stem().string());
                        MaterialDeserializer::Deserialize(grandchild.GetPath().stem().string(), doc);
                    }
                    else
                    {
                        EDITOR_ELOG("Failed to deduce what type of asset type ['{0}'] at path: {1}", attributeValue, grandchild.GetPath().filename().string().c_str());
                    }
                }
            }
        }
    }
}

} // Namespace ScarlettEditor.