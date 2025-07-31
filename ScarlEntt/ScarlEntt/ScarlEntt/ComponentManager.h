#pragma once

// todo Christian try and use ScarlEntt version of assert over this.
#include <ranges>
#include <cassert>
#include <functional>

#include "ScarlEntt.h"
#include "ComponentRef.h"
#include "ComponentView.h"
#include "ComponentArray.h"

namespace ScarlEntt
{

class XmlNode;

/**
 * @class ComponentManager: The owner of the components and instance used to create/destroy and register components.<br/>
 * Components need to be registered before adding or removing.<br/>
 * Once registered, the component manager handles adding/removing/retrieving components for a particular entity.
 */
class ComponentManager
{
friend class EntityHandle;
public:
    ComponentManager() = default;
    ~ComponentManager();

    ComponentManager(const ComponentManager&)               = delete;
    ComponentManager(ComponentManager&&)                    = delete;
    ComponentManager& operator=(ComponentManager&&)         = delete;
    ComponentManager& operator=(const ComponentManager&)    = delete;

    /**
    * @brief Register the component so that the component array can be created and reserved/initialised.
    * @note: __All components need to be registered before they can be used.__
    */
    template <typename ComponentType>
    void RegisterComponent();

    /**
    * @brief Get the array of a specific component type.
    * @see ComponentArray
    * @return Returns the component array of a specific component type.
    */
    template<typename ComponentType>
    [[nodiscard]] ComponentArray<ComponentType>& GetComponentArray();

    /**
     * @brief Get the ID for the component.
     * @tparam ComponentType The component class
     * @return the type ID for the component.
     */
    template <typename ComponentType>
    static inline ComponentTypeId GetComponentTypeId() { return ComponentTypeId{ typeid(ComponentType).name() }; }

#ifdef DEV_CONFIGURATION
    [[nodiscard]] vector<ComponentView>* GetComponents(const EntityId entityId);
#endif // DEV_CONFIGURATION.

private:
    unordered_map<ComponentTypeId, IComponentArray*> mComponents;

#ifdef DEV_CONFIGURATION
    unordered_map <ComponentTypeId, std::function<void(const EntityId, const XmlNode*)>> mComponentTypeToDeserializeFunctionMap;
    unordered_map<EntityId, vector<ComponentView>> mEntityToComponentViewMap;

    template <typename ComponentType>
    void AddComponentViewToMap(const EntityId entityId, const ComponentType& component);
#endif // DEV_CONFIGURATION.

    /**
    * @brief Add a component to an entity.
    * @tparam ComponentType The class of the Component.
    * @tparam Args Arguments that are passed to the ComponentType constructor to construct a component with initial values.
    * @param entityId The ID of the entity the component is being added to.
    * @param args The arguments used to initialise the component.
    * @return A reference to the created component.
    */
    template <typename ComponentType, typename... Args>
    [[maybe_unused]] ComponentType* AddComponent(const EntityId entityId, Args&&... args);

    /**
     * @brief Add a passed in component to the entity.
     * @note The ownership of the __component__ is passed to the ComponentManager once called, therefore, cannot be used after adding to Entity.
     * @tparam ComponentType The class of the Component.
     * @param entityId The ID of the entity the component is being added to.
     * @param component The component that is being added to the component array. Ownership of this component is passed over to ComponentManger after function call.
     * @return A reference to the created component.
     */
    template <typename ComponentType>
    [[maybe_unused]] ComponentType* AddComponent(const EntityId entityId, const ComponentType& component);

    /**
     * Add a component that is deserialized from values in the \c XmlNode.
     * @see XmlNode
     * @param entityId: The ID of the entity the component is being added to.
     * @param node: The \c XmlNode of the component. This should be the node starting with the tag "component".
     */
    void AddComponentFromXml(const EntityId entityId, const XmlNode* node);

    /**
    * @brief Retrieve a pointer to the component of a specific _entity_.
    * @note This should be used if any caching is needed as this retains a reference to the component, and will check if it's valid before use.
    * @see \c ComponentRef
    * @param entityId: The entity ID for which we are requesting the component.
    * @return Returns a \c ComponentRef to a component.
    */
    template <typename ComponentType>
    [[nodiscard]] ComponentRef<ComponentType> GetComponent(const EntityId entityId);

    /**
    * @brief Remove a component (if found) of a specific _entity_.
    * @param entityId: The entity ID for which we are removing the component.
    */
    template <typename ComponentType>
    void RemoveComponent(EntityId entityId);
};

/*
  ======================================================================================================================================================
                                                                                                                                                        */

template <typename ComponentType>
inline void ComponentManager::RegisterComponent()
{
    const ComponentTypeId typeName = GetComponentTypeId<ComponentType>();
    SCARLENTT_ASSERT(!mComponents.contains(typeName) && "Registering component type more than once.");
    assert(!mComponents.contains(typeName) && "Registering component type more than once."); // todo remove.

#ifdef DEV_CONFIGURATION
    mComponentTypeToDeserializeFunctionMap[typeName] = [this](const EntityId entityId, const XmlNode* node) { this->AddComponent(entityId, ComponentType::DeserializeComponent(node)); };
#endif // DEV_CONFIGURATION.

    mComponents[typeName] = new ComponentArray<ComponentType>();
}

template<typename ComponentType>
inline ComponentArray<ComponentType>& ComponentManager::GetComponentArray()
{
    const ComponentTypeId typeName = GetComponentTypeId<ComponentType>();
    SCARLENTT_ASSERT(mComponents.contains(typeName) && "Component not registered before use.");
    assert(mComponents.contains(typeName) && "Component not registered before use."); // todo remove.

    return *static_cast<ComponentArray<ComponentType>*>(mComponents[typeName]);
}

#ifdef DEV_CONFIGURATION
template <typename ComponentType>
void ComponentManager::AddComponentViewToMap(const EntityId entityId, const ComponentType& component)
{
    ComponentRef<ComponentType> componentRef{ entityId, &GetComponentArray<ComponentType>() };

    mEntityToComponentViewMap[entityId].emplace_back(ComponentTypeId{ GetComponentTypeId<ComponentType>() }, [componentRef]() { return componentRef->GetProperties(); });
}
#endif // DEV_CONFIGURATION.

template <typename ComponentType, typename... Args>
inline ComponentType* ComponentManager::AddComponent(const EntityId entityId, Args&&... args)
{
#ifdef DEV_CONFIGURATION
    ComponentType* component = GetComponentArray<ComponentType>().AddComponent(entityId, std::forward<Args>(args)...);
    AddComponentViewToMap(entityId, *component);
    return component;
#else // DEV_CONFIGURATION
    return GetComponentArray<ComponentType>().AddComponent(entityId, std::forward<Args>(args)...);
#endif // Else !DEV_CONFIGURATION.
}

template <typename ComponentType>
inline ComponentType* ComponentManager::AddComponent(const EntityId entityId, const ComponentType& component)
{
#ifdef DEV_CONFIGURATION
    AddComponentViewToMap(entityId, component);
    return GetComponentArray<ComponentType>().AddComponent(entityId, component);
#else // DEV_CONFIGURATION
    return GetComponentArray<ComponentType>().AddComponent(entityId, component);
#endif // Else !DEV_CONFIGURATION.
}

template <typename ComponentType>
inline ComponentRef<ComponentType> ComponentManager::GetComponent(const EntityId entityId)
{
    return ComponentRef<ComponentType>{ entityId, &GetComponentArray<ComponentType>() };
}

template <typename ComponentType>
inline void ComponentManager::RemoveComponent(EntityId entityId)
{
    GetComponentArray<ComponentType>().RemoveComponent(entityId);
}

#ifdef DEV_CONFIGURATION

inline vector<ComponentView>* ComponentManager::GetComponents(const EntityId entityId)
{
    //todo add checks.
    return &mEntityToComponentViewMap[entityId];
}

#endif // DEV_CONFIGURATION.

} // Namespace ScarlEntt
