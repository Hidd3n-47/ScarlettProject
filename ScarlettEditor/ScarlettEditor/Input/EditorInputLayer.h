#pragma once

#include <ScarlettEngine/Core/Input/LayerStack.h>

#include <Core/Events/Event.h>
#include <Core/Events/MouseEvents.h>
#include <Core/Events/KeyboardEvents.h>

#include "Core/Events/ApplicationEvents.h"

namespace ScarlettEditor
{

class EditorInputLayer : public Scarlett::Layer
{
public:
    EditorInputLayer()           = default;
    ~EditorInputLayer() override = default;

    EditorInputLayer(const EditorInputLayer&)               = delete;
    EditorInputLayer(EditorInputLayer&&)                    = delete;
    EditorInputLayer& operator=(EditorInputLayer&&)         = delete;
    EditorInputLayer& operator=(const EditorInputLayer&)    = delete;

protected:
    virtual bool OnMouseButtonPressed(const Scarlett::MouseButtonPressedEvent& e);
    virtual bool OnMouseButtonReleased(const Scarlett::MouseButtonReleasedEvent& e);
    virtual bool OnMouseScrolledEvent(const Scarlett::MouseScrollEvent& e);
    virtual bool OnMouseMoved(const Scarlett::MouseMovedEvent& e);
    virtual bool OnKeyPressed(const Scarlett::KeyPressedEvent& e);
    virtual bool OnKeyReleased(const Scarlett::KeyReleasedEvent& e);
    virtual bool OnKeyTyped(const Scarlett::KeyTypedEvent& e);
    virtual bool OnUpdateEvent(const Scarlett::OnUpdateEvent& e);
private:
    void OnEvent(Scarlett::Event& e) override;

    // Since the event system takes a function pointer, we cannot override methods.
    // Added internal functions to call virtual ones that can be overridden.
    // This prevents input handlers that inherit from this from needing to manually subscribe to events.
    inline bool OnMouseButtonPressedInternal(const Scarlett::MouseButtonPressedEvent& e)    { return OnMouseButtonPressed(e); }
    inline bool OnMouseButtonReleasedInternal(const Scarlett::MouseButtonReleasedEvent& e)  { return OnMouseButtonReleased(e); }
    inline bool OnMouseScrolledEventInternal(const Scarlett::MouseScrollEvent& e)           { return OnMouseScrolledEvent(e); }
    inline bool OnMouseMovedInternal(const Scarlett::MouseMovedEvent& e)                    { return OnMouseMoved(e); }
    inline bool OnKeyPressedInternal(const Scarlett::KeyPressedEvent& e)                    { return OnKeyPressed(e); }
    inline bool OnKeyReleasedInternal(const Scarlett::KeyReleasedEvent& e)                  { return OnKeyReleased(e); }
    inline bool OnKeyTypedInternal(const Scarlett::KeyTypedEvent& e)                        { return OnKeyTyped(e); }
    inline bool OnUpdateEventInternal(const Scarlett::OnUpdateEvent& e)                     { return OnUpdateEvent(e); }

    // todo add some sort of event for panel resizing.
    //virtual bool OnWindowResize(const Scarlett::WindowClosedEvent& e) { return true; }
};

} // Namespace ScarlettEditor.
