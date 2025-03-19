#pragma once

#include "Events/Event.h"

namespace Scarlett
{

class Window;
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

private:
    static Engine* mInstance;

    Window* mMainWindow;

    bool mRunning;

    void OnEvent(Event& e);
    bool OnWindowClose(const WindowClosedEvent& e);
};

Engine* CreateEngine();

} // Namespace Scarlett.
