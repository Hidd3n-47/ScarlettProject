#pragma once

namespace Scarlett
{

class Event;

/**
 * @class MouseEvent: A class used to represent a mouse event.
 */
class MouseEvent : public Event
{
public:
    inline explicit MouseEvent(const float x, const float y)
        : mX(x)
        , mY(y)
    { /* Empty. */ }

    [[nodiscard]] inline float GetXPos() const { return mX; }
    [[nodiscard]] inline float GetYPos() const { return mY; }
protected:
    float mX = 0.0f;
    float mY = 0.0f;
};

/**
 * @class MouseMovedEvent: A class used to represent a mouse moved event.
 */
class MouseMovedEvent final : public MouseEvent
{
public:
    inline MouseMovedEvent(const float x, const float y)
        : MouseEvent(x, y)
    { /* Empty. */ }

    EVENT_CLASS_TYPE(MOUSE_MOVE)
};

/**
 * @class MouseButtonEvent: A class used to represent a mouse button event.
 */
class MouseButtonEvent : public Event
{
public:
    inline explicit MouseButtonEvent(const uint32 button)
        : mButton(button)
    { /* Empty. */ }

    [[nodiscard]] inline uint32 GetMouseButton() const { return mButton; }
protected:
    uint32 mButton;
};

/**
 * @class MouseButtonPressedEvent: A class used to represent a mouse button pressed event.
 */
class MouseButtonPressedEvent final : public MouseButtonEvent
{
public:
    MouseButtonPressedEvent(const uint32 button)
        : MouseButtonEvent{ button }
    { /* Empty. */ }

    EVENT_CLASS_TYPE(MOUSE_BUTTON_PRESSED)
};

/**
 * @class MouseButtonReleasedEvent: A class used to represent a mouse button released event.
 */
class MouseButtonReleasedEvent final : public MouseButtonEvent
{
public:
    MouseButtonReleasedEvent(const uint32 button)
        : MouseButtonEvent{ button }
    { /* Empty. */ }

    EVENT_CLASS_TYPE(MOUSE_BUTTON_RELEASED)
};

} // Namespace Scarlett.
