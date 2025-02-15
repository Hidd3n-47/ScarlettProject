#pragma once

#include <functional>

namespace Scarlett
{

/**
 * @brief Enum to represent the type of event.
 */
enum class EventType
{
    WINDOW_CLOSE,
    MOUSE_MOVE
};

#define EVENT_CLASS_TYPE(type)      [[nodiscard]] static  EventType GetStaticType()               { return EventType::##type; } \
                                    [[nodiscard]] virtual EventType GetEventType() const override { return GetStaticType(); }

/**
 * @class Event: A class that represents an event.
 */
class Event
{
    friend class EventDispatcher;
public:
    Event()             = default;
    virtual ~Event()    = default;

    Event(const Event&)             = default;
    Event(Event&&)                  = default;
    Event& operator=(Event&&)       = default;
    Event& operator=(const Event&)  = default;

    /**
     * Get the type of event @see EventType
     * @return The type of the event.
     */
    [[nodiscard]] virtual EventType GetEventType() const = 0;
    /**
     * Get if the event is handled or not.
     * @return@code true@endcode if the event is handled,@code false@endcode otherwise.
     */
    [[nodiscard]] inline bool IsHandled() const { return mHandled; }

protected:
    bool mHandled = false;
};

/**
 * @class EventDispatcher: A class used to dispatch events to their respective bound function.
 */
class EventDispatcher
{
public:
    inline EventDispatcher(Event& event)
        : mEvent(event)
    { /* Empty*/ }

    /**
     * Dispatch an event class to a respective function callback.
     * @tparam T Generic interface of the events where@code T@endcode inherits from@code Event@endcode.
     * @see Event
     * @param function The callback function the event is being dispatched to.
     * @return@code true@endcode if the event was successfully dispatched to a callback method,@code false@endcode otherwise.
     */
    template<typename T>
    inline bool Dispatch(std::function<bool(const T&)> function)
    {
        if (mEvent.GetEventType() == T::GetStaticType())
        {
            mEvent.mHandled = function(*static_cast<T*>(&mEvent));
            return true;
        }
        return false;
    }

private:
    Event& mEvent;
};

/**
 * @class WindowClosedEvent: A class used to represent a window closed event.
 */
class WindowClosedEvent final : public Event
{
public:
    EVENT_CLASS_TYPE(WINDOW_CLOSE)
};

/**
 * @class MouseEvent: A class used to represent a mouse event.
 */
class MouseEvent : public Event
{
public:
    MouseEvent(const float x, const float y)
        : mX(x)
        , mY(y)
    { /* Empty. */ }

    inline float GetXPos() const { return mX; }
    inline float GetYPos() const { return mY; }
protected:
    float mX = 0.0f;
    float mY = 0.0f;
};

/**
 * @class MouseEvent: A class used to represent a mouse moved event.
 */
class MouseMovedEvent final : public MouseEvent
{
public:
    inline MouseMovedEvent(const float x, const float y)
        : MouseEvent(x, y)
    { /* Empty. */ }

    EVENT_CLASS_TYPE(MOUSE_MOVE)
};

} // Namespace Scarlett.