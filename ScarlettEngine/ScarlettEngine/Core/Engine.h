#pragma once

namespace Scarlett
{

class Window;
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

    void InitEngine();
    void Run() const;
    void DestroyEngine();

private:
    static Engine* mInstance;

    Window* mMainWindow;

    bool mRunning;

    VulkanRendererEditor* mVulkRenderer = nullptr;
};

Engine* CreateEngine();

} // Namespace Scarlett.