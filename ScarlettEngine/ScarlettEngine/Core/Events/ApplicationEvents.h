#pragma once

namespace Scarlett
{

class Event;

/**
* @class WindowClosedEvent: A class used to represent a window closed event.
*/
class WindowClosedEvent final : public Event
{
public:
    EVENT_CLASS_TYPE(WINDOW_CLOSE)
};

/**
* @class OnUpdateEvent: A class used to represent an OnUpdate event.
*/
class OnUpdateEvent final : public Event
{
public:
    EVENT_CLASS_TYPE(ON_UPDATE)
};

} // Namespace Scarlett.
