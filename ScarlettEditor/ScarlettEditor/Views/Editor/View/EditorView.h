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

    void Render() override;

    [[nodiscard]] inline SelectionManager& GetSelectionManager() { return mSelectionManager; }
private:
    void RenderViewport();
    SelectionManager mSelectionManager;

    Panel* mPropertiesPanel;
    Panel* mScenePanel;

    Scarlett::Ref<Scarlett::Overlay> mEditorViewOverlay;
};

} // Namespace ScarlettEditor.
