#include "ScarlettEditorPch.h"
#include "AssetBrowserPanel.h"

#include <imgui/imgui_internal.h>

#include "Core/DirectoryCache.h"

namespace ScarlettEditor
{
AssetBrowserPanel::AssetBrowserPanel(IView* view) : Panel { view, { .title = "Asset Browser" } }//, mDirectory { "E:/Programming/ScarlettProject/Assets/" }
{
    // Create the directory.
    // Todo change it to use the directory cache instead of querying the os for the directories every frame.
    mDirectory = new DirectoryCache{ "E:/Programming/ScarlettProject/Assets/" };
}

void AssetBrowserPanel::Render()
{
#ifdef DEV_CONFIGURATION
    const std::string path = "E:/Programming/ScarlettProject/Assets/";
    // ------------- Tree -------------------
    const float indentSpacing = ImGui::GetStyle().IndentSpacing;
    ImGui::GetStyle().IndentSpacing = 0.0f;

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
    ImGui::TreePush("Scene");

    DrawDirectoryTree(path);

    ImGui::TreePop();
    ImGui::PopStyleVar();

    ImGui::GetStyle().IndentSpacing = indentSpacing;
#endif // DEV_CONFIGURATION.
}

bool AssetBrowserPanel::DrawDirectoryTree(const std::filesystem::path& directoryPath, const uint32 indentAmount)
{
    for (const auto& entry : std::filesystem::directory_iterator{ directoryPath })
    {
        const std::filesystem::path& path = entry.path();
        const std::string name = path.filename().string();

        const bool containsItems = !path.has_extension() && (std::filesystem::directory_iterator(path) != std::filesystem::directory_iterator{});
        const ImGuiTreeNodeFlags flags = containsItems ? 0 : ImGuiTreeNodeFlags_Leaf;

        constexpr uint32 INDENT_SIZE = 12;
        ImGui::Indent(static_cast<float>(INDENT_SIZE * indentAmount));

        if (entry.is_directory())
        {
            bool treeNodeInputHandled = false;

            if (ImGui::TreeNodeEx(name.c_str(), flags))
            {
                treeNodeInputHandled = DrawDirectoryTree(path, indentAmount + 1);
                ImGui::TreePop();
            }

            // todo we should ensure that we open the directory tree node after we add to it.
            if (path == mSelectedDirectory && (mCreateFolder || mCreateFile))
            {
                if (ImGui::TreeNodeEx("##newFileFolderNode", ImGuiTreeNodeFlags_Leaf))
                {
                    ImGui::TreePop();
                }
                ImGui::SameLine();

                char buffer[50] = {};
                bool doneCreating = false;

                // Text input for the new file/folder name.
                if (ImGui::InputText("##newFolder", buffer, 50, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_EditOnCreate))
                {
                    doneCreating = true;
                }

                // Create the new file/folder if the user has completed typing the name.
                if (doneCreating)
                {
                    if (mCreateFolder)
                    {
                        const std::string directoryName{ buffer };

                        create_directory(std::filesystem::path{ path / directoryName });
                        mCreateFolder = false;
                    }
                    else if (mCreateFile)
                    {
                        std::string fileName{ std::filesystem::path{path / buffer}.string() };

                        // Ensure that the file has an extension, and if not, add '.txt'.
                        if (fileName.find('.') == std::string::npos)
                        {
                            fileName += ".txt";
                        }

                        std::ofstream of{ fileName, std::ios::out };
                        of.close();
                        mCreateFile = false;
                    }
                }
            }

            // Check if the tree node has been right-clicked on.
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsItemHovered() && !treeNodeInputHandled)
            {
                mSelectedDirectory = entry;
                ImGui::OpenPopup("AssetBrowserRightClick");
                return true;
            }
        }
        else
        {
            ImGui::BulletText("%s", name.c_str());
        }
        ImGui::Unindent(static_cast<float>(INDENT_SIZE * indentAmount));
    }

    return false;
}

void AssetBrowserPanel::RenderContextMenu()
{
    if (ImGui::BeginPopupContextWindow("AssetBrowserRightClick"))
    {
        mContextOpen = true;
        if (ImGui::MenuItem("New File"))
        {
            mCreateFile = true;
        }
        if (ImGui::MenuItem("New Folder"))
        {
            mCreateFolder = true;
        }
        ImGui::EndPopup();
    }
    else
    {
        mContextOpen = false;
    }
}

} // Namespace ScarlettEditor.
