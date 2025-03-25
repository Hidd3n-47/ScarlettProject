#include "ScarlettEditorPch.h"
#include "EditorView.h"

#include <ScarlEntt/Scene.h>

#include <ScarlettGameCore/Src/GameCore.h>
#include <ScarlettGameCore/Components/Tag.h>
#include <ScarlettGameCore/Components/Transform.h>

#include "Editor/EditorManager.h"
#include "ScarlettGameCore/Components/SquareSprite.h"

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

    ScarlEntt::EntityHandle viewportCamera = ScarlettGame::GameCore::Instance().CreateEntity();
    viewportCamera.RemoveComponent<ScarlettGame::SquareSprite>();
    //viewportCamera.RemoveComponent<ScarlettGame::Tag>(); // TODO remove later, just keeping this as it is handy to see camera position.
    viewportCamera.GetComponent<ScarlettGame::Transform>()->translation = { 0.0f, 0.0f, -5.0f };
    viewportCamera.GetComponent<ScarlettGame::Transform>()->rotation = { 10.0f, 0.0f, 0.0f };
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


