#include "ScarlettEnginePch.h"
#include "WindowManager.h"

#include <glfw/glfw3.h>

namespace Scarlett
{

static void GlfwErrorCallback(int error, const char* description)
{
    SCARLETT_FLOG("{0}", description);
    __debugbreak();
}

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

    glfwSetCursorPosCallback(window, [](GLFWwindow* win, double x, double y)
    {
        WindowProperties& data = *static_cast<WindowProperties*>(glfwGetWindowUserPointer(win));

        if (data.eventCallback)
        {
            MouseMovedEvent event(static_cast<float>(x), static_cast<float>(y));
            data.eventCallback(event);
        }
    });

    return windowHandle;
}

void WindowManager::DestroyWindow(const Window* window)
{
    SCARLETT_DLOG("Destoryed window.");

    delete window;
    window = nullptr;
}


} // Namespace Scarlett.