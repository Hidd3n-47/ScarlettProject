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

void Panel::Begin() const
{
    const char* title = mProperties.title.data();
    ImGui::Begin(title);
}

void Panel::End() const
{
    ImGui::End();
}

} // Namespace ScarlettEditor
