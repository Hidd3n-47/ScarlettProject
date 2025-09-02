#pragma once

#include "Mesh.h"
#include "RenderCommand.h"

namespace Scarlett
{

class Window;

class Renderer
{
public:
    Renderer()          = default;
    virtual ~Renderer() = default;

    Renderer(const Renderer&)               = delete;
    Renderer(Renderer&&)                    = delete;
    Renderer& operator=(Renderer&&)         = delete;
    Renderer& operator=(const Renderer&)    = delete;

    // todo re-look at the asserts. Can't use SCARLETT_ASSERT due to including renderer in other projects such as editor.
    static Renderer* Instance() { assert(mInstance && "Renderer never created. Create renderer before using."); return mInstance.get(); }

    virtual void Init(const Window* windowRef)  = 0;
    virtual void Destroy()                      = 0;

    virtual void BeginRender()  = 0;
    virtual void Render()       = 0;
    virtual void EndRender()    = 0;

    //inline void AddCommand(const RenderType renderType, const RenderCommand& command) { mCommands[renderType].emplace_back(command); }
    inline void AddCommand(const WeakRef<Mesh> mesh, const RenderCommand& command) { mCommands[mesh.GetRawPtr()].emplace_back(command); }

    virtual void OnWindowResize(const uint32 width, const uint32 height) = 0;
protected:
    static std::unique_ptr<Renderer> mInstance;

    const Window* mWindowRef;

    std::unordered_map<Mesh*, vector<RenderCommand>> mCommands;
    //std::unordered_map<RenderType, vector<RenderCommand>> mCommands;
};

} // Namespace Scarlett.
