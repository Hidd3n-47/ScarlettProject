#include "ScarlettEditorPch.h"
#include "SceneSerialization.h"

#include <ScarlettGameCore/Src/GameCore.h>

#include <Serialization/Xml/XmlSerializer.h>

namespace ScarlettEditor
{

//todo change to filepath instead of string.
const std::string SceneSerialization::SCENE_FILE_PATH { "E:/Programming/ScarlettProject/Assets/Scenes/DefaultScene.scarlett_scene" };

void SceneSerialization::SerializeCurrentGameScene()
{
#ifdef DEV_CONFIGURATION
    ScarlEntt::XmlNode* sceneNode = new ScarlEntt::XmlNode("Scene");
    const ScarlEntt::Scene* scene = ScarlettGame::GameCore::Instance().GetActiveScene();
    const auto& entities = scene->GetEntities();

    for (ScarlEntt::EntityHandle handle : entities)
    {
        ScarlEntt::XmlNode* entityNode = new ScarlEntt::XmlNode("Entity");

        for (const auto& component : handle.GetComponents())
        {
            ScarlEntt::XmlNode* componentNode = new ScarlEntt::XmlNode{ "Component" };
            componentNode->AddAttribute("typeId", component.GetComponentTypeId());

            for (auto& [componentTag, componentValue] : *component.GetSerializedValue())
            {
                ScarlEntt::XmlNode* componentValueNode = new ScarlEntt::XmlNode{ componentTag, componentValue.GetValue()};
                componentValueNode->AddAttribute("type", componentValue.GetTypeString());
                componentNode->AddChildNode(componentValueNode);
            }

            entityNode->AddChildNode(componentNode);
        }

        sceneNode->AddChildNode(entityNode);
    }

    ScarlEntt::XmlSerializer::Serialize(ScarlEntt::XmlDocument { sceneNode }, SCENE_FILE_PATH);
#endif // DEV_CONFIGURATION.
}

void SceneSerialization::DeserializeCurrentGameScene()
{
#ifdef DEV_CONFIGURATION
    const ScarlEntt::XmlDocument sceneDocument = ScarlEntt::XmlSerializer::Deserialize(SCENE_FILE_PATH);

    ScarlEntt::Scene* scene = ScarlettGame::GameCore::Instance().GetActiveScene();

    for (ScarlEntt::XmlNode* entityNode : sceneDocument.GetRootNode()->GetChildren())
    {
        ScarlEntt::EntityHandle entity = scene->CreateEntity();
        for (const auto component : entityNode->GetChildren())
        {
            const std::string TYPE_ID_ATTRIBUTE     = "typeId";
            const std::string VALUE_TYPE_ATTRIBUTE  = "name";

            const ScarlEntt::ComponentTypeId componentTypeId { component->GetAttribute(TYPE_ID_ATTRIBUTE) };
            entity.AddDeserializedComponent(componentTypeId, component);
        }
    }

#endif // DEV_CONFIGURATION.
}

} // Namespace ScarlettEditor.
