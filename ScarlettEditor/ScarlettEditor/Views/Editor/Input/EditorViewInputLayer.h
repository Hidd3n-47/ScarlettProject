#pragma once

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

    bool mPanning = false;

    bool OnMouseButtonPressed(const Scarlett::MouseButtonPressedEvent& e) override;
    bool OnMouseButtonReleased(const Scarlett::MouseButtonReleasedEvent& e) override;
    bool OnKeyPressed(const Scarlett::KeyPressedEvent& e) override;
};

} // Namespace ScarlettEditor.
