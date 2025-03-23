#include "ScarlettEditorPch.h"
#include "ViewportPanel.h"

#include <imgui/imgui.h>

#include "Editor/EditorManager.h"

namespace ScarlettEditor
{

void ViewportPanel::Render()
{
    ImGui::Begin("Viewport");

    mIsFocused = ImGui::IsWindowFocused();
    mIsHovered = ImGui::IsWindowHovered();

    const ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    ImGui::Image(EditorManager::Instance().GetViewportTexture(), viewportPanelSize);

    ImGui::End();
}

} // Namespace ScarlettEditor.
