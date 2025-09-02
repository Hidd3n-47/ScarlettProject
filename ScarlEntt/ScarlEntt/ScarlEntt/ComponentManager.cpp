#include "ScarlEnttpch.h"
#include "ComponentManager.h"

#include <ScarlettUtils/Xml/XmlDocument.h>

namespace ScarlEntt
{

ComponentManager::~ComponentManager()
{
    for (const auto& componentArray : mComponents | std::views::values)
    {
        delete componentArray;
    }
}

void ComponentManager::AddComponentFromXml(const EntityId entityId, const ScarlettUtils::XmlElement* node)
{
#ifdef DEV_CONFIGURATION
    const std::string TYPE_ID_ATTRIBUTE = "typeId";
    const ComponentTypeId componentTypeId{ node->GetAttributeValue(TYPE_ID_ATTRIBUTE) };

    assert(mComponentTypeToDeserializeFunctionMap.contains(componentTypeId) && "Trying to deserialize a component type that has not been registered.");

    mComponentTypeToDeserializeFunctionMap[componentTypeId](entityId, node);
#endif // DEV_CONFIGURATION.
}

} // Namespace ScarlEntt.
