#pragma once

#include <Math/Math.h>

#include <Core/Events/Event.h>

#include <Src/PrimitiveTypes.h>


namespace ScarlettEditor
{

class EditorInputManager
{
friend class EditorInputLayer;
public:
    [[nodiscard]] static inline bool IsKeyDown(const uint16 keyCode)                { return mKeyMap[keyCode]; }
    [[nodiscard]] static inline bool IsMouseButtonDown(const uint16 mouseCode)      { return mMouseButtonMap[mouseCode]; }
    [[nodiscard]] static inline ScarlettMath::Vec2 GetMousePosition()               { return mMousePosition; }
private:
    static std::unordered_map<uint32, bool> mKeyMap;
    static std::unordered_map<uint32, bool> mMouseButtonMap;

    static ScarlettMath::Vec2 mMousePosition;

    static void OnEvent(Scarlett::Event& e);
};

} // Namespace ScarlettEditor.
