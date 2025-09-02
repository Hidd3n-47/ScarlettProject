#include "ScarlettEditorPch.h"
#include "DirectoryCache.h"

namespace ScarlettEditor
{

DirectoryCache::DirectoryCache(std::filesystem::path directoryStartingPath)
    : mPath{ std::move(directoryStartingPath) }
    , mIsDirectory{ !mPath.has_extension() }
{
    if (is_directory(mPath))
    {
        AddChild(mPath);
    }
}

void DirectoryCache::AddChild(const std::filesystem::path& path)
{
    for (const auto& entry : std::filesystem::directory_iterator{ path })
    {
        mChildren.emplace_back(entry);
    }
}

} // Namespace ScarlettEditor.
