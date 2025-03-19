#pragma once

#include "IView.h"

#include "Editor/SelectionManager.h"

namespace ScarlEntt
{
class Scene;
}

namespace ScarlettEditor
{

class Panel;

class EditorView final : public IView
{
public:
    EditorView();
    ~EditorView() override;

    void Render() override;

    [[nodiscard]] SelectionManager& GetSelectionManager() { return mSelectionManager; }
private:
    void RenderViewport() const;
    SelectionManager mSelectionManager;

    Panel* mPropertiesPanel;
    Panel* mScenePanel;
};

} // Namespace ScarlettEditor.
