#pragma once

namespace Scarlett
{

/**
 * @class Filepath: Filepath is a class that can be used to create paths for files relative to the project folder.
 * @note Filepath represents a file and hence must have an extension. A filepath is not a directory.
 */
class Filepath
{
public:
    Filepath()  = default;
    ~Filepath() = default;
    explicit Filepath(std::filesystem::path path) : mPath{ std::move(path) } { /* Empty. */ }

    Filepath(const Filepath&)               = default;
    Filepath(Filepath&&)                    = default;
    Filepath& operator=(const Filepath&)    = default;
    Filepath& operator=(Filepath&&)         = default;

    /**
     * @brief Get the file name of the \c Filepath. Note, the file name includes extensions.
     * @return The filename of the \c Filepath.
     */
    [[nodiscard]] inline std::string GetFileName() const { return mPath.filename().string(); }

    /**
     * Get the absolute (full) filepath of the file on disk.
     * @return The absolute (full) filepath of the file on disk.
     */
    [[nodiscard]] inline std::string GetAbsolutePath() const { if (mPath.is_relative()) return std::string{ SCARLETT_PROJECT_DIRECTORY } + mPath.string(); return mPath.string(); }
    /**
     * Get the extension of the file.
     * @return The extension of the file.
     */
    [[nodiscard]] inline std::string GetExtension() const { return mPath.extension().string(); }
    /**
     * Check if the path exists on disk and is valid.
     * @return \c True if the path exists on disk, \c false otherwise.
     */
    [[nodiscard]] inline bool DoesPathExist() const { return std::filesystem::exists(GetAbsolutePath()); }

    [[nodiscard]] inline bool operator==(const Filepath& other) const { return mPath == other.mPath; }

    static constexpr std::string_view SCARLETT_PROJECT_DIRECTORY{ "E:/Programming/ScarlettProject/" };
private:
    std::filesystem::path mPath;
};

} // Namespace Scarlett.

template <>
struct std::hash<Scarlett::Filepath>
{
    [[nodiscard]] inline size_t operator()(const Scarlett::Filepath& path) const noexcept
    {
        return std::hash<std::string>()(path.GetAbsolutePath());
    }
};
