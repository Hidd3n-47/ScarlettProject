#pragma once

#include <memory>

namespace Scarlett
{

class Window;

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
    void DestroyEngine() const;

private:
    static Engine* mInstance;

    Window* mMainWindow;

    bool mRunning;
};

Engine* CreateEngine();

} // Namespace Scarlett.