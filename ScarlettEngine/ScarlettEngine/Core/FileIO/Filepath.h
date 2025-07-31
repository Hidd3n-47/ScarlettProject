#pragma once

namespace Scarlett
{

/**
 * @class Filepath: Filepath is a class that can be used to create paths relative to the project folder.
 */
class Filepath
{
public:
    explicit Filepath(std::string path) : mPath{ std::move(path) } { /* Empty. */ }

    [[nodiscard]] inline std::string GetAbsolutePath() const { return std::string{ mProjectPath } + mPath; }
    [[nodiscard]] inline std::string GetExtension() const { const size_t dotPos = mPath.find_last_of('.'); return { mPath.substr(dotPos + 1, mPath.length() - dotPos) }; }
private:
    std::string mPath;

    // Todo Why can't be constexpr?
    const std::string_view mProjectPath{ "E:/Programming/ScarlettProject/" };
};

} // Namespace Scarlett.