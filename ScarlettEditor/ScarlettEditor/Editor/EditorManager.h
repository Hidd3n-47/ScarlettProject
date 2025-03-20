#pragma once

#include <imgui/imgui.h>

#include "Src/ScarlettEditor.h"

namespace ScarlettEditor
{

class IView;

class EDITOR_API EditorManager
{
public:
    static inline EditorManager& Instance() { return *mInstance; }

    static void Init(ImGuiContext* uiContext, ImGuiMemAllocFunc* allocateFunction, ImGuiMemFreeFunc* freeFunction, void* data);
    static void Destroy();

    void RenderUi(const ImTextureID viewportTexture);

    [[nodiscard]] ImTextureID           GetViewportTexture()     const { return mViewportTexture; }
private:
    static EditorManager* mInstance;

    ImTextureID mViewportTexture = 0;

    // Todo, fix from being static and made in the init.
    static IView*      mCurrentView;
};

} // Namespace ScarlettEditor.
