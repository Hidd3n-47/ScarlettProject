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

    static Renderer& Instance() { SCARLETT_ASSERT(mInstance && "Renderer never created. Create renderer before using."); return *mInstance.get(); }

    virtual void Init(const Window* windowRef) = 0;
    virtual void Destroy() = 0;

    virtual void BeginRender() = 0;
    virtual void Render() = 0;
    virtual void EndRender() = 0;

    // Todo Change Renderer to use commands instead and remove references to these.
    virtual ScarlettGame::Camera* GetRenderCamera() = 0;
    inline void AddCommand(const RenderType renderType, RenderCommand* command) { mCommands[renderType].push_back(command); };

    virtual void OnWindowResize(const uint32 width, const uint32 height) = 0;
protected:
    static std::unique_ptr<Renderer> mInstance;

    const Window* mWindowRef;

    std::unordered_map<RenderType, vector<RenderCommand*>> mCommands;
};

} // Namespace Scarlett.
