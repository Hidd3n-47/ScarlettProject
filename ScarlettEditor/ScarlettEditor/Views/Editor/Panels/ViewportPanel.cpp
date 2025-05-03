#include "ScarlettEditorPch.h"
#include "ViewportPanel.h"

#include <imgui/imgui.h>

#include "Editor/EditorManager.h"

namespace ScarlettEditor
{

void ViewportPanel::Render()
{
    const ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    ImGui::Image(EditorManager::Instance().GetViewportTexture(), viewportPanelSize);
}

} // Namespace ScarlettEditor.
