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

    void OnEvent(Scarlett::Event& e) override;
private:
    EditorView* mEditorView = nullptr;

    bool mCameraFlying = false;

    // todo this capture should not exist, and mouse down should be able to capture it.
    bool mCaptureMouseDownPos = false;
    ScarlettMath::Vec2 mMouseButtonDownPosition { -1.f };
    ScarlettMath::Vec3 mOriginalRotation        { 0.0f }; // todo change this so that we don't have to capture this but rather work on a per frame basis.

    bool OnMouseButtonPressed(const Scarlett::MouseButtonPressedEvent& e) override;
    bool OnMouseButtonReleased(const Scarlett::MouseButtonReleasedEvent& e) override;

    bool OnMouseMoved(const Scarlett::MouseMovedEvent& e) override;

    bool OnKeyPressed(const Scarlett::KeyPressedEvent& e) override;
};

} // Namespace ScarlettEditor.
