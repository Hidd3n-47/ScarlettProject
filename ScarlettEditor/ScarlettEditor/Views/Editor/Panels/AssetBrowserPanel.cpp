#include "ScarlettEditorPch.h"
#include "AssetBrowserPanel.h"

#include <imgui/imgui_internal.h>

#include "Core/DirectoryCache.h"
#include "Core/AssetManagement/AssetManager.h"

namespace ScarlettEditor
{
AssetBrowserPanel::AssetBrowserPanel(IView* view) : Panel { view, { .title = "Asset Browser" } }
{
    // Create the directory.
    // Todo change it to use the directory cache instead of querying the os for the directories every frame.
    mDirectory = new DirectoryCache{ AssetManager::ASSET_PATH };
}

void AssetBrowserPanel::Render()
{
#ifdef DEV_CONFIGURATION
    // ------------- Tree -------------------
    const float indentSpacing = ImGui::GetStyle().IndentSpacing;
    ImGui::GetStyle().IndentSpacing = 0.0f;

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
    ImGui::TreePush("Scene");

    DrawDirectoryTree(AssetManager::ASSET_PATH);

    ImGui::TreePop();
    ImGui::PopStyleVar();

    ImGui::GetStyle().IndentSpacing = indentSpacing;
#endif // DEV_CONFIGURATION.
}

void AssetBrowserPanel::RenderContextMenu()
{
    if (ImGui::BeginPopupContextWindow("AssetBrowserRightClick"))
    {
        mContextOpen = true;
        if (ImGui::MenuItem("New Folder"))
        {
            mCreateFolder = true;
        }
        if (ImGui::BeginMenu("Create Asset"))
        {
            for (uint32 i{ 0 }; i < static_cast<uint32>(AssetTypes::COUNT); ++i)
            {
                if (const AssetTypes type = static_cast<AssetTypes>(i); ImGui::MenuItem(AssetManager::AssetTypeToString(type).c_str()))
                {
                    mCreateFile = true;
                    mAssetTypeToCreate = type;
                }
            }
            ImGui::EndMenu();
        }
        ImGui::EndPopup();
    }
    else
    {
        mContextOpen = false;
    }
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
                        CreateAssetFromSelectedType(std::filesystem::path{ path / buffer });

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

void AssetBrowserPanel::CreateAssetFromSelectedType(const std::filesystem::path& path) const
{
    const std::string fileName{ path.string() };
    std::string extension;

    switch (mAssetTypeToCreate)
    {
    case AssetTypes::MATERIAL_FILE:
        extension = AssetManager::SCARLETT_ASSET_EXTENSION;
        break;
    default:
    case AssetTypes::TEXT_FILE:
        extension = ".txt";
        break;
    }

    std::ofstream of{ fileName + extension, std::ios::out };
    of.close();
}

} // Namespace ScarlettEditor.
