#pragma once

struct GLFWwindow;

namespace Scarlett
{

/**
 * @enum WindowRenderApi
 * An enum used to determine which rendering API is being used.
 */
enum class WindowRenderApi
{
    NO_API
};

/**
 * @class WindowProperties
 * A structure to hold the properties of a window.
 */
struct WindowProperties
{
    WindowProperties() = default;
    ~WindowProperties() = default;

    WindowProperties(const WindowProperties&) = default;
    WindowProperties(WindowProperties&&) = default;
    WindowProperties& operator=(WindowProperties&&) = default;
    WindowProperties& operator=(const WindowProperties&) = default;

    /** The text displayed in the titlebar. */
    std::string_view title = "Scarlett Engine";

    /** The width of the window.            */
    uint32  width = 1920;
    /** The height of the window.           */
    uint32  height = 1080;
    /** If the window is resizable.         */
    bool    resizable = false;
    /** If the window contains a titlebar.  */
    bool    titleBar = true;

    /** The API used to render to the window. */
    WindowRenderApi renderApi = WindowRenderApi::NO_API;
};

/**
 * @class Window
 * A class to hold a handle to a window that is API agnostic.<br/>
 * Use @see WindowManager to create and destroy a window.
 */
class Window
{
public:
    Window(GLFWwindow* windowHandle, const WindowProperties& windowProperties);

    /**
     * @brief Update the window.
     */
    void Update() const;

    /**
     * @brief Get the width of the window.
     * @return The width of the window.
     */
    [[nodiscard]] inline uint32             GetWidth()          const   { return mWindowProperties.width; }
    /**
     * @brief Get the height of the window.
     * @return The height of the window.
     */
    [[nodiscard]] inline uint32             GetHeight()         const   { return mWindowProperties.height; }
    /**
     * @brief Get a handle to the native window.
     * @return A pointer to the memory of the window.
     */
    [[nodiscard]] inline void*              GetNativeWindow()   const   { return mWindowHandle; }
    /**
     * @brief Get the properties of the window.
     * @see WindowProperties
     * @return Properties of the window.
     */
    [[nodiscard]] inline WindowProperties*  GetProperties()             { return &mWindowProperties; }
private:
    GLFWwindow*         mWindowHandle;

    WindowProperties    mWindowProperties;
};

} // Namespace Scarlett.