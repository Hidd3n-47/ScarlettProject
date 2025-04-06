#include "ScarlettEnginePch.h"
#include "InputManager.h"

#include "Core/Events/MouseEvents.h"
#include "Core/Events/KeyboardEvents.h"

namespace Scarlett
{

std::unordered_map<uint32, bool>    InputManager::mKeyMap           = { };
std::unordered_map<uint32, bool>    InputManager::mMouseButtonMap   = { };
ScarlettMath::Vec2                  InputManager::mMousePosition    = { 0.0f, 0.0f };

void InputManager::OnEvent(Event& e)
{
    EventDispatcher dispatcher(e);

    dispatcher.Dispatch<KeyPressedEvent>([](const KeyPressedEvent& event)
    {
        mKeyMap[event.GetKeyCode()] = true;
        return false;
    });

    dispatcher.Dispatch<KeyReleasedEvent>([](const KeyReleasedEvent& event)
    {
        mKeyMap[event.GetKeyCode()] = false;
        return false;
    });

    dispatcher.Dispatch<MouseButtonPressedEvent>([](const MouseButtonPressedEvent& event)
    {
        mMouseButtonMap[event.GetMouseButton()] = true;
        return false;
    });

    dispatcher.Dispatch<MouseButtonReleasedEvent>([](const MouseButtonReleasedEvent& event)
    {
        mMouseButtonMap[event.GetMouseButton()] = false;
        return false;
    });

    dispatcher.Dispatch<MouseMovedEvent>([](const MouseMovedEvent& event)
    {
        mMousePosition = { event.GetXPos(), event.GetYPos() };
        return false;
    });
}

} // Namespace Scarlett.
