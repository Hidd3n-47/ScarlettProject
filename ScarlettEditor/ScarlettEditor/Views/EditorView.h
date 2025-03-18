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

    inline void SetCurrentScene(ScarlEntt::Scene* currentScene) { mCurrentScene = currentScene; }
private:
    void RenderViewport() const;
    void RenderProperties() const;
    void RenderSceneGraph();

    SelectionManager mSelectionManager;
    ScarlEntt::Scene* mCurrentScene = nullptr;
    Panel* mPropertiesPanel;
    Panel* mScenePanel;
};

} // Namespace ScarlettEditor.
