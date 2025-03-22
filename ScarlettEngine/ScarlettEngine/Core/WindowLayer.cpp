#include "ScarlettEnginePch.h"
#include "WindowLayer.h"

#include "Engine.h"

#include "Events/Event.h"
#include "Events/ApplicationEvents.h"

namespace Scarlett
{

void WindowLayer::OnEvent(Event& e)
{
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowClosedEvent>(SCARLETT_BIND_FUNCTION(WindowLayer::OnWindowClose));
}

bool WindowLayer::OnWindowClose(const WindowClosedEvent& e)
{
    Engine::Instance().WindowClosed();
    return true;
}


} // Namespace Scarlett.
