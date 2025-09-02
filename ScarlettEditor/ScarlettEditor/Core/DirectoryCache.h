#pragma once

namespace ScarlettEditor
{

class DirectoryCache
{
public:
    explicit DirectoryCache(std::filesystem::path directoryStartingPath);

    void AddChild(const std::filesystem::path& path);

    [[nodiscard]] inline bool IsDirectory()         const { return mIsDirectory; }
    [[nodiscard]] inline std::string GetExtension() const { return mPath.extension().string(); }

    [[nodiscard]] inline const std::filesystem::path&   GetPath()                 const { return mPath; }
    [[nodiscard]] inline const vector<DirectoryCache>&  GetDirectoryChildren()    const { return mChildren; }
private:
    std::filesystem::path   mPath;
    bool                    mIsDirectory;

    vector<DirectoryCache>  mChildren;
};

} // Namespace ScarlettEditor.