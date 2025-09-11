#include "ScarlettEditorPch.h"
#include "SceneSerialization.h"

#include <ScarlettGameCore/Src/GameCore.h>

#include <ScarlettUtils/Xml/XmlSerializer.h>

#include "Components/BoundingBox.h"
#include "Components/Tag.h"

namespace ScarlettEditor
{

//todo change to filepath instead of string.
const std::string SceneSerialization::SCENE_FILE_PATH { "E:/Programming/ScarlettProject/Assets/Scenes/DefaultScene.scarlett_scene" };

void SceneSerialization::SerializeCurrentGameScene()
{
#ifdef DEV_CONFIGURATION
    ScarlettUtils::XmlElement* sceneNode = new ScarlettUtils::XmlElement("Scene");
    const ScarlEntt::Scene* scene = ScarlettGame::GameCore::Instance().GetActiveScene();
    const auto& entities = scene->GetEntities();

    for (const ScarlEntt::EntityHandle& handle : entities)
    {
        ScarlettUtils::XmlElement entityNode{ "Entity" };

        for (auto& component : *handle.GetComponents())
        {
            std::string componentAttributeString = "typeId=\"" + component.GetComponentTypeId()->Type() + "\"";
            ScarlettUtils::XmlElement componentNode{ "Component", componentAttributeString };

            for (auto& [componentTag, componentProperty] : *component.GetProperties())
            {
                const std::string propertyValue = componentProperty.GetPropertyValue();
                std::string attributeString = "type=\"" + componentProperty.GetTypeAsString() + "\"";
                componentNode.AddChild(componentTag, attributeString, propertyValue);
            }

            entityNode.AddChild(componentNode);
        }

        sceneNode->AddChild(entityNode);
    }

    ScarlettUtils::XmlSerializer::Serialize(ScarlettUtils::XmlDocument { sceneNode }, SCENE_FILE_PATH);
#endif // DEV_CONFIGURATION.
}

void SceneSerialization::DeserializeCurrentGameScene()
{
#ifdef DEV_CONFIGURATION
    const ScarlettUtils::XmlDocument sceneDocument = ScarlettUtils::XmlSerializer::Deserialize(SCENE_FILE_PATH);

    ScarlEntt::Scene* scene = ScarlettGame::GameCore::Instance().GetActiveScene();

    for (const ScarlettUtils::XmlElement& entityNode : sceneDocument.GetRootElement()->GetChildElements())
    {
        ScarlEntt::EntityHandle entity = scene->CreateEntity();
        for (const auto& component : entityNode.GetChildElements())
        {
            entity.AddComponentFromXml(&component);
        }

        const auto tag = entity.GetComponent<Scarlett::Component::Tag>();
        if (tag.IsValid())
        {
            tag->entity = ScarlEntt::EntityHandle{ entity };
        }
    }
#endif // DEV_CONFIGURATION.
}

} // Namespace ScarlettEditor.
