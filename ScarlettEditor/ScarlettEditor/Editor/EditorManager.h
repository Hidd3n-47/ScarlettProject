#pragma once

#include <imgui/imgui.h>

#include <Core/Types/WeakRef.h>

#include "Src/ScarlettEditor.h"
#include "Core/MaterialManager/EditorMaterialManager.h"

namespace Scarlett
{
class Event;
class LayerStack;
}

namespace ScarlettEditor
{

class IView;
class AssetManager;

class EDITOR_API EditorManager
{
public:
    EditorManager() = default;
    ~EditorManager();

     EditorManager(const  EditorManager&)               = delete;
     EditorManager( EditorManager&&)                    = delete;
     EditorManager& operator=( EditorManager&&)         = delete;
     EditorManager& operator=(const  EditorManager&)    = delete;

    static inline EditorManager& Instance() { return *mInstance; }

    static void CreateInstance(ImGuiContext* uiContext, ImGuiMemAllocFunc* allocateFunction, ImGuiMemFreeFunc* freeFunction, void* data);
    static void DestroyInstance();

    void Init(const Scarlett::WeakRef<EditorMaterialManager> materialManagerRef);

    void OnEvent(Scarlett::Event& e) const;

    void RenderUi(const ImTextureID viewportTexture);

    [[nodiscard]] inline ImTextureID           GetViewportTexture()    const { return mViewportTexture; }
    [[nodiscard]] inline Scarlett::LayerStack* GetLayerStack()         const { return mEditorLayerStack; }
    [[nodiscard]] inline IView*                GetCurrentView()        const { return mCurrentView; }
    [[nodiscard]] inline AssetManager*         GetAssetManager()       const { return mAssetManager; }

    [[nodiscard]] inline Scarlett::WeakRef<EditorMaterialManager> GetMaterialManager() const { return mMaterialManagerRef; }
private:
    static EditorManager*   mInstance;

    ImTextureID             mViewportTexture      = 0;

    Scarlett::LayerStack*   mEditorLayerStack   = nullptr;
    IView*                  mCurrentView        = nullptr;

    AssetManager*           mAssetManager       = nullptr;

    Scarlett::WeakRef<EditorMaterialManager> mMaterialManagerRef;
};

} // Namespace ScarlettEditor.
