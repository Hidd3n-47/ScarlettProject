#pragma once

#include "RenderCommand.h"

namespace ScarlettGame
{
struct Camera;
}

namespace Scarlett {

class Window;

class Renderer
{
public:
    Renderer()          = default;
    virtual ~Renderer() = default;

    // todo re-look at the assert. Can't use SCARLETT_ASSERT due to including renderer in other projects such as editor.
    static Renderer& Instance() { assert(mInstance && "Renderer never created. Create renderer before using."); return *mInstance; }

    virtual void Init(const Window* windowRef)  = 0;
    virtual void Destroy()                      = 0;

    virtual void BeginRender()  = 0;
    virtual void Render()       = 0;
    virtual void EndRender()    = 0;

    inline void AddCommand(const RenderType renderType, const RenderCommand& command) { mCommands[renderType].emplace_back(command); }

    virtual void OnWindowResize(const uint32 width, const uint32 height) = 0;
protected:
    static std::unique_ptr<Renderer> mInstance;

    const Window* mWindowRef;

    std::unordered_map<RenderType, vector<RenderCommand>> mCommands;
};

} // Namespace Scarlett.
