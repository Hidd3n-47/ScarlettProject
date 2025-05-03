#pragma once

// todo Christian try and use ScarlEntt version of assert over this.
#include <cassert>
#include <ranges>

#include <stdexcept>
#include "Debug.h"

#include "ComponentArray.h"
#include "ScarlEntt.h"

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
    inline const vector<ComponentTypeId>& GetRegisteredComponentTypes() const { return mRegisteredComponentTypes; }
#endif // DEV_CONFIGURATION.
private:
    unordered_map<ComponentTypeId, IComponentArray*> mComponents;

    /**
     * Deserialize a component from the XML node and add it to the entity.
     * @param entityId The ID of the entity the component is being added to.
     * @param componentTypeId The ID of the component being added.
     * @param node The XML node of the component that contains the values to deserialize into the component.
     */
    void AddDeserializedComponent(const EntityId entityId, const ComponentTypeId& componentTypeId, XmlNode* node);

    /**
    * @brief Add a component to an entity.
    * @param componentTypeId The type ID of the Component.
    * @param entityId The ID of the entity the component is being added to.
    */
    void AddDefaultComponent(const ComponentTypeId& componentTypeId, const EntityId entityId);

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

#ifdef DEV_CONFIGURATION
    vector<ComponentTypeId>                           mRegisteredComponentTypes;
    unordered_map<EntityId, vector<ComponentView>>    mEntityToComponentMap;

    unordered_map<ComponentTypeId, std::function<void(const EntityId)>>             mDefaultComponentFunctionMap;
    unordered_map<ComponentTypeId, std::function<void(const EntityId, XmlNode*)>>   mDeserializeComponentFunctionMap;

    /**
    * @brief Retrieve a vector of \c ComponentView for the components the requested entity.
    * @note This is only available in Dev Configuration and should only be used for debugging.
    * @see \c ComponentView
    * @param entityId: The entity ID for which we are requesting the components.
    * @return Returns a \c ComponentView for each of the components on the entity.
    */
    inline const vector<ComponentView>& GetComponents(const EntityId entityId) { return mEntityToComponentMap[entityId]; }
#endif // DEV_CONFIGURATION.
};

/*
  ======================================================================================================================================================
                                                                                                                                                        */

inline ComponentManager::~ComponentManager()
{
    for (const auto& componentArray : mComponents | std::views::values)
    {
        delete componentArray;
    }
}

template <typename ComponentType>
inline void ComponentManager::RegisterComponent()
{
    const ComponentTypeId typeName = GetComponentTypeId<ComponentType>();
    SCARLENTT_ASSERT(!mComponents.contains(typeName) && "Registering component type more than once.");
    assert(!mComponents.contains(typeName) && "Registering component type more than once."); // todo remove.

#ifdef DEV_CONFIGURATION
    mRegisteredComponentTypes.emplace_back(typeName);
    mDefaultComponentFunctionMap[typeName]     = [this](const EntityId entityId) { AddComponent(entityId, ComponentType{}); };
    mDeserializeComponentFunctionMap[typeName] = [this](const EntityId entityId, XmlNode* node){ AddComponent(entityId, ComponentType::DeserializeComponent(node)); };
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

inline void ComponentManager::AddDeserializedComponent(const EntityId entityId, const ComponentTypeId& componentTypeId, XmlNode* node)
{
    mDeserializeComponentFunctionMap[componentTypeId](entityId, node);
}

inline void ComponentManager::AddDefaultComponent(const ComponentTypeId& componentTypeId, const EntityId entityId)
{
    mDefaultComponentFunctionMap[componentTypeId](entityId);
}

template <typename ComponentType, typename... Args>
inline ComponentType* ComponentManager::AddComponent(const EntityId entityId, Args&&... args)
{
#ifdef DEV_CONFIGURATION
    // We need to construct the component before we can create the reference.
    ComponentType* component = GetComponentArray<ComponentType>().AddComponent(entityId, std::forward<Args>(args)...);

    vector<ComponentView>& entityComponents = mEntityToComponentMap[entityId];
    if (std::find(entityComponents.begin(), entityComponents.end(), GetComponentTypeId<ComponentType>().Type()) == entityComponents.end())
    {
        ComponentRef<ComponentType> componentRef { entityId, &GetComponentArray<ComponentType>() };
        entityComponents.emplace_back(GetComponentTypeId<ComponentType>().Type(), [componentRef]() { return componentRef->GetSerializedFunction(); });
    }
    else
    {
        throw std::runtime_error("Component Type already added to entity.");
    }

    return component;
#else // DEV_CONFIGURATION.
    return GetComponentArray<ComponentType>().AddComponent(entityId, std::forward<Args>(args)...);
#endif // Else DEV_CONFIGURATION.
}

template <typename ComponentType>
inline ComponentType* ComponentManager::AddComponent(const EntityId entityId, const ComponentType& component)
{
#ifdef DEV_CONFIGURATION
    // We need to construct the component before we can create the reference.
    ComponentType* c = GetComponentArray<ComponentType>().AddComponent(entityId, component);

    vector<ComponentView>& entityComponents = mEntityToComponentMap[entityId];
    if (std::find(entityComponents.begin(), entityComponents.end(), GetComponentTypeId<ComponentType>().Type()) == entityComponents.end())
    {
        ComponentRef<ComponentType> componentRef { entityId, &GetComponentArray<ComponentType>() };
        const ComponentTypeId componentTypeId = GetComponentTypeId<ComponentType>();
        entityComponents.emplace_back(componentTypeId.Type(), [componentRef]() { return componentRef->GetSerializedFunction(); });
    }
    else
    {
        throw std::runtime_error("Component Type already added to entity.");
    }

    return c;
#else // DEV_CONFIGURATION
    return GetComponentArray<ComponentType>().AddComponent(entityId, component);
#endif // Else DEV_CONFIGURATION.
}

template <typename ComponentType>
inline ComponentRef<ComponentType> ComponentManager::GetComponent(const EntityId entityId)
{
    return ComponentRef<ComponentType>{ entityId, &GetComponentArray<ComponentType>() };
}

template <typename ComponentType>
inline void ComponentManager::RemoveComponent(EntityId entityId)
{
#ifdef DEV_CONFIGURATION
    vector<ComponentView>& entityComponents = mEntityToComponentMap[entityId];
    if (const auto it = std::find(entityComponents.begin(), entityComponents.end(), GetComponentTypeId<ComponentType>().Type()); it != entityComponents.end())
    {
        entityComponents.erase(it);
    }
    else
    {
        throw std::runtime_error("Component Type already added to entity.");
    }
#endif // DEV_CONFIGURATION.

    GetComponentArray<ComponentType>().RemoveComponent(entityId);
}

} // Namespace ScarlEntt
