#pragma once

#include <Core/Input/Layer.h>
#include <Core/Events/Event.h>

#include <Core/Events/KeyboardEvents.h>
#include <Src/ScarlettEngineDefines.h>

namespace Scarlett
{

class EmptyLayer final : public Layer { };

class StaticKeyPressVariableStorage
{
public:
    inline static void Reset()
    {
        mKeyBeenPressedFromLayer = false;
        mKeyBeenPressedFromOverlay = false;
    }

    static void KeyPressedFromLayer()   { mKeyBeenPressedFromLayer = true; }
    static void KeyPressedFromOverlay() { mKeyBeenPressedFromOverlay = true; }

    [[nodiscard]] static bool HasKeyBeenPressedFromLayer()   { return mKeyBeenPressedFromLayer; }
    [[nodiscard]] static bool HasKeyBeenPressedFromOverlay() { return mKeyBeenPressedFromOverlay; }
private:
    static bool mKeyBeenPressedFromLayer;
    static bool mKeyBeenPressedFromOverlay;
};

bool StaticKeyPressVariableStorage::mKeyBeenPressedFromLayer       = false;
bool StaticKeyPressVariableStorage::mKeyBeenPressedFromOverlay     = false;

class EmptyLayerForKeyPress final : public Layer
{
public:
    inline void OnEvent(Event& e) override
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<KeyPressedEvent>(SCARLETT_BIND_FUNCTION(EmptyLayerForKeyPress::OnKeyPressedEvent));
    }

    inline static bool OnKeyPressedEvent(const KeyPressedEvent& e)
    {
        StaticKeyPressVariableStorage::KeyPressedFromLayer();
        return true;
    }
};

class EmptyOverlayForKeyPress final : public Layer
{
public:
    inline void OnEvent(Event& e) override
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<KeyPressedEvent>(SCARLETT_BIND_FUNCTION(EmptyOverlayForKeyPress::OnKeyPressedEvent));
    }

    inline static bool OnKeyPressedEvent(const KeyPressedEvent& e)
    {
        StaticKeyPressVariableStorage::KeyPressedFromOverlay();
        return true;
    }
};

class EmptyOverlayForKeyPressThatDoesntHandle final : public Layer
{
public:
    inline void OnEvent(Event& e) override
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<KeyPressedEvent>(SCARLETT_BIND_FUNCTION(EmptyOverlayForKeyPressThatDoesntHandle::OnKeyPressedEvent));
    }

    inline static bool OnKeyPressedEvent(const KeyPressedEvent& e)
    {
        StaticKeyPressVariableStorage::KeyPressedFromOverlay();
        return false;
    }
};


} // Namespace Scarlett.
