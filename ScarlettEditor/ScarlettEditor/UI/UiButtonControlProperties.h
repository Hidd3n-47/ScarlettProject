#pragma once

#include <imgui/imgui.h>

namespace ScarlettEditor
{

struct UiButtonControlProperties
{
    ImVec2 buttonSize           = { 5.0f, 5.0f };
    ImVec4 buttonColor          = { 1.0f, 0.0f, 1.0f, 1.0f };
    ImVec4 buttonHoveredColor   = { 1.0f, 0.0f, 1.0f, 1.0f };
};

} // Namespace ScarlettEditor.