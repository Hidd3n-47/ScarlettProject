#pragma once

namespace Scarlett {

class Window;

class Renderer
{
public:
    Renderer() = default;
    virtual ~Renderer() = default;

    virtual void Init(const Window* windowRef) = 0;
    virtual void Destroy() = 0;

    virtual void BeginRender() = 0;
    virtual void Render() = 0;
    virtual void EndRender() = 0;

    virtual void OnWindowResize(const uint32 width, const uint32 height) = 0;
protected:
    const Window* mWindowRef;
};

} // Namespace Scarlett.