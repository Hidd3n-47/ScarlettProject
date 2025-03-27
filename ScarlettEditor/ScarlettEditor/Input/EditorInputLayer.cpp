#include "ScarlettEditorPch.h"
#include "EditorInputLayer.h"

#include <imgui/imgui.h>

namespace ScarlettEditor
{

void EditorInputLayer::OnEvent(Scarlett::Event& e)
{
    Scarlett::EventDispatcher dispatcher(e);
    dispatcher.Dispatch<Scarlett::MouseButtonPressedEvent>(SCARLETT_BIND_FUNCTION(EditorInputLayer::OnMouseButtonPressedInternal));
    dispatcher.Dispatch<Scarlett::MouseButtonReleasedEvent>(SCARLETT_BIND_FUNCTION(EditorInputLayer::OnMouseButtonReleasedInternal));
    dispatcher.Dispatch<Scarlett::MouseScrollEvent>(SCARLETT_BIND_FUNCTION(EditorInputLayer::OnMouseScrolledEventInternal));
    dispatcher.Dispatch<Scarlett::MouseMovedEvent>(SCARLETT_BIND_FUNCTION(EditorInputLayer::OnMouseMovedInternal));

    dispatcher.Dispatch<Scarlett::KeyPressedEvent>(SCARLETT_BIND_FUNCTION(EditorInputLayer::OnKeyPressedInternal));
    dispatcher.Dispatch<Scarlett::KeyReleasedEvent>(SCARLETT_BIND_FUNCTION(EditorInputLayer::OnKeyReleasedInternal));
    dispatcher.Dispatch<Scarlett::KeyTypedEvent>(SCARLETT_BIND_FUNCTION(EditorInputLayer::OnKeyTypedInternal));

    dispatcher.Dispatch<Scarlett::OnUpdateEvent>(SCARLETT_BIND_FUNCTION(EditorInputLayer::OnUpdateEventInternal));
}

bool EditorInputLayer::OnMouseButtonPressed(const Scarlett::MouseButtonPressedEvent& e)
{
    ImGuiIO& io = ImGui::GetIO();
    io.MouseDown[e.GetMouseButton()] = true;
    return true;
}

bool EditorInputLayer::OnMouseButtonReleased(const Scarlett::MouseButtonReleasedEvent& e)
{
    ImGuiIO& io = ImGui::GetIO();
    io.MouseDown[e.GetMouseButton()] = false;
    return true;
}

bool EditorInputLayer::OnMouseMoved(const Scarlett::MouseMovedEvent& e)
{
    // ImGui automatically captures the mouse moved event.
    return true;
}

bool EditorInputLayer::OnMouseScrolledEvent(const Scarlett::MouseScrollEvent& e)
{
    ImGuiIO& io = ImGui::GetIO();
    io.AddMouseWheelEvent(e.GetXOffset(), e.GetYOffset());
    return true;

}

bool EditorInputLayer::OnKeyPressed(const Scarlett::KeyPressedEvent& e)
{
    ImGuiIO& io = ImGui::GetIO();
    io.AddKeyEventScarlett(e.GetKeyCode(), true);
    return true;
}

bool EditorInputLayer::OnKeyReleased(const Scarlett::KeyReleasedEvent& e)
{
    ImGuiIO& io = ImGui::GetIO();
    io.AddKeyEventScarlett(e.GetKeyCode(), false);
    return true;
}

bool EditorInputLayer::OnKeyTyped(const Scarlett::KeyTypedEvent& e)
{
    ImGuiIO& io = ImGui::GetIO();
    io.AddInputCharacter(e.GetKeyCode());
    return true;
}

bool EditorInputLayer::OnUpdateEvent(const Scarlett::OnUpdateEvent& e)
{
    return true;
}

} // Namespace ScarlettEditor.
