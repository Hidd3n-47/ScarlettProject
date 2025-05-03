#include "ScarlettEditorPch.h"
#include "Panel.h"

#include "imgui/imgui.h"

namespace ScarlettEditor
{

void Panel::RenderUi()
{
    Begin();
    Render();
    End();
}

void Panel::Begin()
{
    const char* title = mProperties.title.data();
    ImGui::Begin(title);

    RenderContextMenu();

    mIsFocused = ImGui::IsWindowFocused();
    mIsHovered = ImGui::IsWindowHovered();
}

void Panel::End() const
{
    ImGui::End();
}

} // Namespace ScarlettEditor
