#include "ScarlettEditorPch.h"
#include "EditorInputLayer.h"

#include <imgui/imgui.h>

namespace ScarlettEditor
{

void EditorInputLayer::OnEvent(Scarlett::Event& e)
{
    Scarlett::EventDispatcher dispatcher(e);
    dispatcher.Dispatch<Scarlett::MouseButtonPressedEvent>(SCARLETT_BIND_FUNCTION(EditorInputLayer::OnMouseButtonPressed));
    dispatcher.Dispatch<Scarlett::MouseButtonReleasedEvent>(SCARLETT_BIND_FUNCTION(EditorInputLayer::OnMouseButtonReleased));
    dispatcher.Dispatch<Scarlett::MouseMovedEvent>(SCARLETT_BIND_FUNCTION(EditorInputLayer::OnMouseMoved));
    dispatcher.Dispatch<Scarlett::MouseScrollEvent>(SCARLETT_BIND_FUNCTION(EditorInputLayer::OnMouseScrolledEvent));
    dispatcher.Dispatch<Scarlett::KeyPressedEvent>(SCARLETT_BIND_FUNCTION(EditorInputLayer::OnKeyPressed));
    dispatcher.Dispatch<Scarlett::KeyReleasedEvent>(SCARLETT_BIND_FUNCTION(EditorInputLayer::OnKeyReleased));
    dispatcher.Dispatch<Scarlett::KeyTypedEvent>(SCARLETT_BIND_FUNCTION(EditorInputLayer::OnKeyTyped));
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

} // Namespace ScarlettEditor.
