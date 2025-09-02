#pragma once

#include "Types/WeakRef.h"

namespace Scarlett
{

class Event;
class Layer;
class Window;
class LayerStack;
class WindowLayer;

class VulkanRenderer;
class VulkanRendererEditor;

class MeshManager;
class TextureManager;
class MaterialManager;

class Engine
{
public:
    Engine()            = default;
    virtual ~Engine()   = default;

    Engine(const Engine&)               = delete;
    Engine(Engine&&)                    = delete;
    Engine& operator=(Engine&&)         = delete;
    Engine& operator=(const Engine&)    = delete;

    static inline Engine& Instance() { return *mInstance; }

    void InitEngine();
    void Run() const;
    void DestroyEngine();

    inline void WindowClosed() { mRunning = false; }

    [[nodiscard]] inline WeakRef<MeshManager>       GetMeshManager()    const { return WeakRef{ mMeshManager }; }
    [[nodiscard]] inline WeakRef<TextureManager>    GetTextureManager() const { return WeakRef{ mTextureManager }; }
private:
    static Engine* mInstance;

    Window* mMainWindow     = nullptr;
    LayerStack* mLayerStack = nullptr;

    WeakRef<Layer> mWindowLayer;

    MeshManager*        mMeshManager;
    TextureManager*     mTextureManager;
    MaterialManager*    mMaterialManager;

    bool mRunning;

    void OnEvent(Event& e) const;
};

Engine* CreateEngine();

} // Namespace Scarlett.
