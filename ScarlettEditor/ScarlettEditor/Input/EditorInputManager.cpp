#include "ScarlettEditorPch.h"
#include "EditorInputManager.h"

#include <Core/Events/MouseEvents.h>
#include <Core/Events/KeyboardEvents.h>

namespace ScarlettEditor
{

std::unordered_map<uint32, bool>    EditorInputManager::mKeyMap           = { };
std::unordered_map<uint32, bool>    EditorInputManager::mMouseButtonMap   = { };
ScarlettMath::Vec2                  EditorInputManager::mMousePosition    = { 0.0f, 0.0f };

void EditorInputManager::OnEvent(Scarlett::Event& e)
{
    Scarlett::EventDispatcher dispatcher(e);

    dispatcher.Dispatch<Scarlett::KeyPressedEvent>([](const Scarlett::KeyPressedEvent& event)
    {
        mKeyMap[event.GetKeyCode()] = true;
        return false;
    });

    dispatcher.Dispatch<Scarlett::KeyReleasedEvent>([](const Scarlett::KeyReleasedEvent& event)
    {
        mKeyMap[event.GetKeyCode()] = false;
        return false;
    });

    dispatcher.Dispatch<Scarlett::MouseButtonPressedEvent>([](const Scarlett::MouseButtonPressedEvent& event)
    {
        mMouseButtonMap[event.GetMouseButton()] = true;
        return false;
    });

    dispatcher.Dispatch<Scarlett::MouseButtonReleasedEvent>([](const Scarlett::MouseButtonReleasedEvent& event)
    {
        mMouseButtonMap[event.GetMouseButton()] = false;
        return false;
    });

    dispatcher.Dispatch<Scarlett::MouseMovedEvent>([](const Scarlett::MouseMovedEvent& event)
    {
        mMousePosition = { event.GetXPos(), event.GetYPos() };
        return false;
    });
}
}
