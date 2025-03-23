#pragma once

#define SCARLETT_BIND_FUNCTION(fn)[this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Scarlett
{

/**
 * @brief Enum to represent the type of event.
 */
enum class EventType
{
    WINDOW_CLOSE,

    MOUSE_MOVE,
    MOUSE_SCROLL,
    MOUSE_BUTTON_PRESSED,
    MOUSE_BUTTON_RELEASED,

    KEY_PRESSED,
    KEY_RELEASED,
    KEY_TYPED
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
     * @return \c true if the event is handled, \c false otherwise.
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
    inline explicit EventDispatcher(Event& event)
        : mEvent(event)
    { /* Empty*/ }

    /**
     * Dispatch an event class to a respective function callback.
     * @tparam T Generic interface of the events where \c T inherits from \c Event.
     * @see Event
     * @param function The callback function the event is being dispatched to.
     * @return \c true if the event was successfully dispatched to a callback method, \c false otherwise.
     */
    template<typename T>
    inline bool Dispatch(const std::function<bool(const T&)>& function)
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

} // Namespace Scarlett.
