#pragma once

#include "Window.h"

namespace Scarlett
{

/**
 * @class WindowManager: A class used to manage the creation and destruction of windows
 * as well as the initialization and termination of the API used to create windows.
 */
class WindowManager
{
public:
    /**
     * @brief Initialize the API used to create windows.<br/>
     * Note: API must be initialized before a window can be made or destroyed.
     */
    static void InitApi();

    /**
     * @brief Terminate the API used to create windows.
     */
    static void TerminateApi();

    /**
     * @brief Create a window.
     * @param windowProperties The properties that created window will have.
     * @see WindowProperties
     * @return A handle to the created window.
     */
    static Window*  CreateWindowInternal(const WindowProperties& windowProperties = WindowProperties());

    /**
     * @brief Destroy a window.
     * @param window The widow that is being destroyed. <br/>
     * Note the window will become@code nullptr@endcode after called.
     */
    static void     DestroyWindow(const Window* window);

private:
    static bool mInitialized;
};

} // Namespace Scarlett.