#pragma once

#include "Types/Ref.h"

namespace Scarlett
{

class Event;
class Layer;
class Window;
class LayerStack;
class WindowLayer;
class VulkanRenderer;
class VulkanRendererEditor;

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
private:
    static Engine* mInstance;

    Window* mMainWindow     = nullptr;
    LayerStack* mLayerStack = nullptr;

    Ref<Layer> mWindowLayer;

    bool mRunning;

    void OnEvent(Event& e) const;
};

Engine* CreateEngine();

} // Namespace Scarlett.
