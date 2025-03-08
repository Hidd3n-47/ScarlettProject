#pragma once

#include "Src/ScarlettEditor.h"

#include <imgui/imgui.h>

namespace ScarlettEditor
{

class EDITOR_API EditorManager
{
public:
    static inline EditorManager& Instance() { return *mInstance; }

    static void Init(ImGuiContext* uiContext, ImGuiMemAllocFunc* allocateFunction, ImGuiMemFreeFunc* freeFunction, void* data);
    static void Destroy();

    void RenderUi(const ImTextureID viewportTexture);
private:
    static EditorManager* mInstance;
};

} // Namespace ScarlettEditor.
