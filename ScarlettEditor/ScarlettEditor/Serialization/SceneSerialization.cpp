#include "ScarlettEditorPch.h"
#include "SceneSerialization.h"

#include <ScarlettGameCore/Src/GameCore.h>

#include <Serialization/Xml/XmlSerializer.h>

#include "Components/BoundingBox.h"
#include "Components/Tag.h"

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

    for (const ScarlEntt::EntityHandle& handle : entities)
    {
        ScarlEntt::XmlNode* entityNode = new ScarlEntt::XmlNode("Entity");

        for (auto& component : *handle.GetComponents())
        {
            ScarlEntt::XmlNode* componentNode = new ScarlEntt::XmlNode{ "Component" };
            componentNode->AddAttribute("typeId", component.GetComponentTypeId()->Type());

            for (auto& [componentTag, componentValue] : *component.GetProperties())
            {
                const std::string propertyValue = componentValue.GetPropertyValue();
                ScarlEntt::XmlNode* componentValueNode = new ScarlEntt::XmlNode{ componentTag, propertyValue };
                componentValueNode->AddAttribute("type", componentValue.GetTypeAsString());
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

    for (const ScarlEntt::XmlNode* entityNode : sceneDocument.GetRootNode()->GetChildren())
    {
        ScarlEntt::EntityHandle entity = scene->CreateEntity();
        for (const auto component : entityNode->GetChildren())
        {
            entity.AddComponentFromXml(component);
        }

        auto tag = entity.GetComponent<Scarlett::Component::Tag>();
        if (tag.IsValid())
        {
            tag->entity = ScarlEntt::EntityHandle{ entity };
        }
    }

#endif // DEV_CONFIGURATION.
}

} // Namespace ScarlettEditor.
