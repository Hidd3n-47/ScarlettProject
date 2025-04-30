#include "ScarlettEditorPch.h"
#include "EditorView.h"

#include <ScarlEntt/Scene.h>

#include <ScarlettGameCore/Src/GameCore.h>

#include <Components/Transform.h>
#include <Components/SquareSprite.h>

#include "Serialization/SceneSerialization.h"

#include "Editor/EditorManager.h"

#include "Views/Editor/Panels/ScenePanel.h"
#include "Views/Editor/Panels/ViewportPanel.h"
#include "Views/Editor/Panels/PropertiesPanel.h"


#include "Views/Editor/Input/EditorViewInputLayer.h"

#include <Components/Line.h>

namespace ScarlettEditor
{

EditorView::EditorView()
{
    SceneSerialization::DeserializeCurrentGameScene();

    AddPanel<ScenePanel>();
    AddPanel<ViewportPanel>();
    AddPanel<PropertiesPanel>();

    mEditorViewOverlay = EditorManager::Instance().GetLayerStack()->PushOverlay<EditorViewInputLayer>(this);
}

EditorView::~EditorView()
{
    SceneSerialization::SerializeCurrentGameScene();

    EditorManager::Instance().GetLayerStack()->PopOverlay(mEditorViewOverlay);
}

} // ScarlettEditor.


