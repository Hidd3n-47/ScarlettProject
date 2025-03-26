#include "ScarlettEnginePch.h"
#include "WindowManager.h"

#include <glfw/glfw3.h>

#include "Core/Events/MouseEvents.h"
#include "Core/Events/KeyboardEvents.h"
#include "Core/Events/ApplicationEvents.h"

namespace Scarlett
{

namespace
{

void GlfwErrorCallback(int error, const char* description)
{
    SCARLETT_FLOG("{0}", description);
    __debugbreak();
}

} // Anonymous namespace.

bool WindowManager::mInitialized = false;

void WindowManager::InitApi()
{
    mInitialized = true;

    const int glfwSuccess = glfwInit();
    glfwSetErrorCallback(GlfwErrorCallback);

    SCARLETT_ASSERT(glfwSuccess && "Failed to initialize glfw.");

    SCARLETT_DLOG("Initialized GLFW (Window API).");
}

void WindowManager::TerminateApi()
{
    glfwTerminate();

    SCARLETT_DLOG("Terminated GLFW (Window API).");
}

Window* WindowManager::CreateWindowInternal(const WindowProperties& windowProperties /* = WindowProperties() */)
{
    SCARLETT_ASSERT(mInitialized && "Initialize Window API before creating the window.");

    if (windowProperties.renderApi == WindowRenderApi::NO_API)
    {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    }

    glfwWindowHint(GLFW_RESIZABLE, windowProperties.resizable);

    GLFWwindow* window = glfwCreateWindow(static_cast<int>(windowProperties.width), static_cast<int>(windowProperties.height), windowProperties.title.data(), nullptr, nullptr);

    SCARLETT_DLOG("{0} x {1} Window Created.", windowProperties.width, windowProperties.height);

    Window* windowHandle = new Window(window, windowProperties);
    glfwSetWindowUserPointer(window, windowHandle->GetProperties());

    glfwSetWindowCloseCallback(window, [](GLFWwindow* win)
    {
        const WindowProperties& data = *static_cast<WindowProperties*>(glfwGetWindowUserPointer(win));

        if (data.eventCallback)
        {
            WindowClosedEvent event;
            data.eventCallback(event);
        }
    });

    glfwSetCursorPosCallback(window, [](GLFWwindow* win, const double x, const double y)
    {
        const WindowProperties& data = *static_cast<WindowProperties*>(glfwGetWindowUserPointer(win));

        if (data.eventCallback)
        {
            MouseMovedEvent event(static_cast<float>(x), static_cast<float>(y));
            data.eventCallback(event);
        }
    });

    glfwSetScrollCallback(window, [](GLFWwindow* win, const double xOffset, const double yOffset)
    {
        const WindowProperties& data = *static_cast<WindowProperties*>(glfwGetWindowUserPointer(win));

        MouseScrollEvent event(static_cast<float>(xOffset), static_cast<float>(yOffset));
        data.eventCallback(event);
    });

    glfwSetMouseButtonCallback(window, [](GLFWwindow* win, const int button, const int action, const int mods)
    {
        const WindowProperties& data = *static_cast<WindowProperties*>(glfwGetWindowUserPointer(win));

        switch (action)
        {
        case GLFW_PRESS:
        {
            MouseButtonPressedEvent event(button);
            data.eventCallback(event);
            break;
        }
        case GLFW_RELEASE:
        {
            MouseButtonReleasedEvent event(button);
            data.eventCallback(event);
            break;
        }
        default:
            SCARLETT_WLOG("Mouse callback action that is not captured.");
            break;
        }
    });

    glfwSetKeyCallback(window, [](GLFWwindow* win, const int key, const int scanCode, const int action, const int mods)
    {
        const WindowProperties& data = *static_cast<WindowProperties*>(glfwGetWindowUserPointer(win));

        switch(action)
        {
        case GLFW_PRESS:
        {
            KeyPressedEvent event(key);
            data.eventCallback(event);
            break;
        }
        case GLFW_RELEASE:
        {
            KeyReleasedEvent event(key);
            data.eventCallback(event);
            break;
        }
        case GLFW_REPEAT:
        {
            KeyRepeatEvent event(key, 1);
            data.eventCallback(event);
            break;
        }
        default:
            SCARLETT_WLOG("Key callback action that is not captured.");
            break;
        }
    });

    glfwSetCharCallback(window, [](GLFWwindow* win, const uint32 keycode)
    {
        const WindowProperties& data = *static_cast<WindowProperties*>(glfwGetWindowUserPointer(win));

        KeyTypedEvent event(keycode);
        data.eventCallback(event);
    });

    return windowHandle;
}

void WindowManager::DestroyWindow(const Window* window)
{
    SCARLETT_DLOG("Destroyed window.");

    delete window;
    window = nullptr;
}

} // Namespace Scarlett.
