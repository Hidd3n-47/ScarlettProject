#include "ScarlettEditorPch.h"
#include "EditorView.h"

#include <ScarlEntt/Scene.h>

#include <ScarlettGameCore/Src/GameCore.h>
#include <ScarlettGameCore/Components/Tag.h>

#include "Editor/EditorManager.h"

#include "Views/Editor/Panels/ScenePanel.h"
#include "Views/Editor/Panels/ViewportPanel.h"
#include "Views/Editor/Panels/PropertiesPanel.h"

#include "Views/Editor/ViewportCameraSystem.h"

#include "Views/Editor/Input/EditorViewInputLayer.h"

namespace ScarlettEditor
{

EditorView::EditorView()
{
    ScarlettGame::GameCore::Instance().GetActiveScene()->RegisterSystem<ViewportCameraSystem>();
    ScarlettGame::GameCore::Instance().GetActiveScene()->RegisterComponent<ViewportCamera>();

    const ScarlEntt::EntityHandle viewportCamera = ScarlettGame::GameCore::Instance().CreateEntity();
    viewportCamera.RemoveComponent<ScarlettGame::Tag>();
    (void)viewportCamera.AddComponent<ViewportCamera>();

    AddPanel<ScenePanel>();
    AddPanel<ViewportPanel>();
    AddPanel<PropertiesPanel>();

    mEditorViewOverlay = EditorManager::Instance().GetLayerStack()->PushOverlay<EditorViewInputLayer>(this);
}

EditorView::~EditorView()
{
    EditorManager::Instance().GetLayerStack()->PopOverlay(mEditorViewOverlay);
}

} // ScarlettEditor.


