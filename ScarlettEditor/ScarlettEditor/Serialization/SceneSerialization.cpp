#include "ScarlettEditorPch.h"
#include "SceneSerialization.h"

#include "Serialization/Xml/XmlSerializer.h"

namespace ScarlettEditor
{

const std::string SceneSerialization::SCENE_FILE_PATH { "E:/Programming/ScarlettProject/Assets/Scenes/DefaultScene.scarlett_scene" };

void SceneSerialization::SerializeCurrentGameScene()
{

}

void SceneSerialization::DeserializeCurrentGameScene()
{
    XmlDocument sceneDocument = XmlSerializer::Deserialize(SCENE_FILE_PATH);
}

} // Namespace ScarlettEditor.
