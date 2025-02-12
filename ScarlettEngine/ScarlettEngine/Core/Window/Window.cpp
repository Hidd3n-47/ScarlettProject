#include "ScarlettEnginePch.h"
#include "Window.h"

#include <glfw/glfw3.h>

namespace Scarlett
{

Window::Window(GLFWwindow* windowHandle, const WindowProperties& windowProperties)
    : mWindowHandle(windowHandle)
    , mWindowProperties(windowProperties)
{
    // Empty.
}

void Window::Update() const
{
    glfwPollEvents();
}

} // Namespace Scarlett.