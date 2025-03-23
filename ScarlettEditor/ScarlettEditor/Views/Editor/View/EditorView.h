#pragma once

#include <ScarlettEngine/Core/Input/LayerStack.h>

#include "Views/IView.h"

#include "Editor/SelectionManager.h"

namespace ScarlEntt
{
class Scene;
} // Namespace Scene.

namespace ScarlettEditor
{

class Panel;

class EditorView final : public IView
{
friend class EditorViewInputLayer;
public:
    EditorView();
    ~EditorView() override;

    EditorView(const EditorView&)               = delete;
    EditorView(EditorView&&)                    = delete;
    EditorView& operator=(EditorView&&)         = delete;
    EditorView& operator=(const EditorView&)    = delete;

    [[nodiscard]] inline SelectionManager& GetSelectionManager() { return mSelectionManager; }
private:
    SelectionManager mSelectionManager;

    Scarlett::WeakRef<Scarlett::Overlay> mEditorViewOverlay;
};

} // Namespace ScarlettEditor.
