#include "ScarlettEditorPch.h"
#include "SceneSerialization.h"

#include <ScarlettGameCore/Src/GameCore.h>

#include "Serialization/Xml/XmlSerializer.h"

namespace ScarlettEditor
{

const std::string SceneSerialization::SCENE_FILE_PATH { "E:/Programming/ScarlettProject/Assets/Scenes/DefaultScene.scarlett_scene" };

void SceneSerialization::SerializeCurrentGameScene()
{
#ifdef DEV_CONFIGURATION
    XmlNode* sceneNode = new XmlNode("Scene");
    const ScarlEntt::Scene* scene = ScarlettGame::GameCore::Instance().GetActiveScene();
    const auto& entities = scene->GetEntities();

    for (ScarlEntt::EntityHandle handle : entities)
    {
        XmlNode* entityNode = new XmlNode("Entity");

        for (const auto& component : handle.GetComponents())
        {
            XmlNode* componentNode = new XmlNode{ component.GetComponentTypeId() };

            auto map = component.GetSerializedValue();
            for (auto& [componentTag, componentValue] : *map)
            {
                componentNode->AddChildNode(new XmlNode{ componentTag, componentValue });
            }

            entityNode->AddChildNode(componentNode);
        }

        sceneNode->AddChildNode(entityNode);
    }

    XmlSerializer::Serialize(XmlDocument { sceneNode }, SCENE_FILE_PATH);
#endif // DEV_CONFIGURATION.
}

void SceneSerialization::DeserializeCurrentGameScene()
{
#ifdef DEV_CONFIGURATION
    XmlDocument sceneDocument = XmlSerializer::Deserialize(SCENE_FILE_PATH);
#endif // DEV_CONFIGURATION.
}

} // Namespace ScarlettEditor.
