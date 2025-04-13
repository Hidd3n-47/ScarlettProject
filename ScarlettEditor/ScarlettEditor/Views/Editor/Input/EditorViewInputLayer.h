#pragma once

#include <Math/Math.h>

#include "Input/EditorInputLayer.h"

namespace ScarlettEditor
{

class EditorView;

class EditorViewInputLayer final : public EditorInputLayer
{
public:
    explicit EditorViewInputLayer(EditorView* editorView)
        : mEditorView(editorView)
    { /* Empty. */ }

private:
    EditorView* mEditorView = nullptr;

    bool    mCameraFlying               = false;
    float   mCameraFlyingMultiplier     = 1.0f;

    ScarlettMath::Vec2 mPreviousMousePosition   { -1.f };

    bool OnMouseButtonPressed(const Scarlett::MouseButtonPressedEvent& e) override;
    bool OnMouseButtonReleased(const Scarlett::MouseButtonReleasedEvent& e) override;

    bool OnMouseMoved(const Scarlett::MouseMovedEvent& e) override;
    bool OnMouseScrolledEvent(const Scarlett::MouseScrollEvent& e) override;

    bool OnKeyPressed(const Scarlett::KeyPressedEvent& e) override;
    bool OnKeyReleased(const Scarlett::KeyReleasedEvent& e) override;

    bool OnUpdateEvent(const Scarlett::OnUpdateEvent& e) override;
};

} // Namespace ScarlettEditor.
