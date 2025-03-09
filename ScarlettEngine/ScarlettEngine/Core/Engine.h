#pragma once

#include "Events/Event.h"

namespace ScarlEntt
{
class Scene;
}

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

    [[nodiscard]] ScarlEntt::Scene* GetCurrentScene() const { return mScene; }
private:
    static Engine* mInstance;

    Window* mMainWindow;

    bool mRunning;

    void OnEvent(Event& e);
    bool OnWindowClose(const WindowClosedEvent& e);

    ScarlEntt::Scene* mScene;
};

Engine* CreateEngine();

} // Namespace Scarlett.
