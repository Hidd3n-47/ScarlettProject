#pragma once

namespace Scarlett
{

class Event;

/**
 * @class KeyEvent: A class used to represent a keyboard event.
 */
class KeyEvent : public Event
{
public:
    inline explicit KeyEvent(const uint32 keyCode)
        : mKeyCode{ keyCode }
    { /* Empty. */ }

    [[nodiscard]] inline uint32 GetKeyCode() const { return mKeyCode; }
protected:
    uint32 mKeyCode;
};

/**
 * @class KeyPressedEvent: A class used to represent a key pressed event.
 */
class KeyPressedEvent final : public KeyEvent
{
public:
    inline KeyPressedEvent(const uint32 keyCode)
        : KeyEvent{ keyCode }
    { /* Empty. */ }

    EVENT_CLASS_TYPE(KEY_PRESSED)
};

/**
* @class KeyRepeatEvent: A class used to represent a key repeated event. Repeated happens once the key is held down.
*/
class KeyRepeatEvent final : public KeyEvent
{
public:
    inline KeyRepeatEvent(const uint32 keyCode, const uint32 repeatCount)
        : KeyEvent{ keyCode }
    , mRepeatCount(repeatCount)
    { /* Empty. */ }

    [[nodiscard]] inline uint32 GetRepeatCount() const { return mRepeatCount; }

    EVENT_CLASS_TYPE(KEY_REPEAT)
private:
    uint32 mRepeatCount;
};

/**
 * @class KeyReleasedEvent: A class used to represent a key released event.
 */
class KeyReleasedEvent final : public KeyEvent
{
public:
    inline KeyReleasedEvent(const uint32 keyCode)
        : KeyEvent{ keyCode }
    { /* Empty. */ }

    EVENT_CLASS_TYPE(KEY_RELEASED)
};

/**
 * @class KeyTypedEvent: A class used to represent a key typed event.
 */
class KeyTypedEvent final : public KeyEvent
{
public:
    inline explicit KeyTypedEvent(const uint32 keyCode)
    : KeyEvent{ keyCode }
    { /* Empty. */ }

    EVENT_CLASS_TYPE(KEY_TYPED)
};

} // Namespace Scarlett.
