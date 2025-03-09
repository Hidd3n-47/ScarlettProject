#pragma once

#include <imgui/imgui.h>

#include "Src/ScarlettEditor.h"

namespace ScarlEntt
{
    class Scene;
}

namespace ScarlettEditor
{

class IView;

class EDITOR_API EditorManager
{
public:
    static inline EditorManager& Instance() { return *mInstance; }

    static void Init(ImGuiContext* uiContext, ImGuiMemAllocFunc* allocateFunction, ImGuiMemFreeFunc* freeFunction, void* data);
    static void Destroy();

    void RenderUi(ScarlEntt::Scene* currentScene, const ImTextureID viewportTexture);

    [[nodiscard]] ImTextureID           GetViewportTexture()     const { return mViewportTexture; }
    [[nodiscard]] ScarlEntt::Scene*     GetCurrentScene()        const { return mCurrentScene; }
private:
    static EditorManager* mInstance;

    ImTextureID mViewportTexture = 0;

    IView*              mCurrentView    = nullptr;
    ScarlEntt::Scene*   mCurrentScene   = nullptr;
};

} // Namespace ScarlettEditor.
