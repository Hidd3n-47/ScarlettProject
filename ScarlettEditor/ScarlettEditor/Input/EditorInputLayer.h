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

    virtual void OnEvent(Scarlett::Event& e) override;
protected:
    virtual bool OnMouseButtonPressed(const Scarlett::MouseButtonPressedEvent& e);
    virtual bool OnMouseButtonReleased(const Scarlett::MouseButtonReleasedEvent& e);
    virtual bool OnMouseScrolledEvent(const Scarlett::MouseScrollEvent& e);
    virtual bool OnMouseMoved(const Scarlett::MouseMovedEvent& e);

    virtual bool OnKeyPressed(const Scarlett::KeyPressedEvent& e);
    virtual bool OnKeyReleased(const Scarlett::KeyReleasedEvent& e);
    virtual bool OnKeyTyped(const Scarlett::KeyTypedEvent& e);

    virtual inline bool OnUpdateEvent(const Scarlett::OnUpdateEvent& e);

    // todo add some sort of event for panel resizing.
    //virtual bool OnWindowResize(const Scarlett::WindowClosedEvent& e) { return true; }
};

} // Namespace ScarlettEditor.
