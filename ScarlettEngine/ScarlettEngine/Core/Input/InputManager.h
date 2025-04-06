#pragma once

#include <Math/Math.h>

#include "Core/Events/Event.h"

#include "Src/PrimitiveTypes.h"

namespace Scarlett
{

class InputManager
{
friend class Engine;
public:
    /**
     * @brief Get if a key is down.
     * @param keyCode The key code being checked.
     * @return \c True if the key is down, \c false otherwise.
     */
    [[nodiscard]] static inline bool IsKeyDown(const uint16 keyCode)            { return mKeyMap[keyCode]; }
    /**
     * @brief Get if a mouse button is down.
     * @param mouseCode The mosue button code being checked.
     * @return \c True if the key is down, \c false otherwise.
     */
    [[nodiscard]] static inline bool IsMouseButtonDown(const uint16 mouseCode)  { return mMouseButtonMap[mouseCode]; }
    /**
     * @brief Get the position of the mouse cursor.
     * @return The position of the vector.
     */
    [[nodiscard]] static inline ScarlettMath::Vec2 GetMousePosition()           { return mMousePosition; }
private:
    static std::unordered_map<uint32, bool> mKeyMap;
    static std::unordered_map<uint32, bool> mMouseButtonMap;

    static ScarlettMath::Vec2 mMousePosition;

    static void OnEvent(Event& e);
};

} // Namespace Scarlett.
