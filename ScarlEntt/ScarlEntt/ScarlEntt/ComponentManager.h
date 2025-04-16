#pragma once

// todo Christian try and use ScarlEntt version of assert over this.
#include <cassert>
#include <ranges>
#include <string>

#include "ComponentArray.h"
#include "ScarlEntt.h"

namespace ScarlEntt
{

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
    * @brief Register the component so that the component array can be created and reserved/initialized.
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

private:
    unordered_map<std::string, IComponentArray*> mComponents;

    /**
    * @brief Add a component to an entity.
    * @tparam ComponentType The class of the Component
    * @tparam Args Arguments that are passed to the ComponentType constructor to construct a component with initial values.
    * @param entityId The ID of the entity the component is being added to.
    * @param args The arguments used to initialize the component.
    * @return A reference to the created component.
    */
    template <typename ComponentType, typename... Args>
    [[maybe_unused]] ComponentType* AddComponent(const EntityId entityId, Args&&... args);

    /**
     * @brief Add a passed in component to the entity.
     * @note The ownership of the __component__ is passed to the ComponentManager once called, therefore cannot be used after adding to Entity.
     * @tparam ComponentType The class of the Component.
     * @param entityId The ID of the entity the component is being added to.
     * @param component The component that is being added to the component array. Ownership of this component is passed over to ComponentManger after function call.
     * @return A reference to the created component.
     */
    template <typename ComponentType>
    [[maybe_unused]] ComponentType* AddComponent(const EntityId entityId, const ComponentType& component);

    /**
    * @brief Retrieve a pointer to the component of a specific _entity_.
    * @note This should be used if any caching is needed as this retains a reference to the component, and will check if its valid before use.
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
    const char* id = typeid(ComponentType).name();
    const std::string typeName(id);
    //ComponentType::TypeName(typeName); // TODO remove if we don't need.
    SCARLENTT_ASSERT(!mComponents.contains(typeName) && "Registering component type more than once.");
    assert(!mComponents.contains(typeName) && "Registering component type more than once."); // todo remove.

    mComponents[typeName] = new ComponentArray<ComponentType>();
}

template<typename ComponentType>
inline ComponentArray<ComponentType>& ComponentManager::GetComponentArray()
{
    const char* id = typeid(ComponentType).name();
    const std::string typeName(id);
    //ComponentType::TypeName(typeName); // TODO remove if we don't need.
    SCARLENTT_ASSERT(mComponents.contains(typeName) && "Component not registered before use.");
    assert(mComponents.contains(typeName) && "Component not registered before use."); // todo remove.

    return *static_cast<ComponentArray<ComponentType>*>(mComponents[typeName]);
}

template <typename ComponentType, typename... Args>
inline ComponentType* ComponentManager::AddComponent(const EntityId entityId, Args&&... args)
{
    return GetComponentArray<ComponentType>().AddComponent(entityId, std::forward<Args>(args)...);
}

template <typename ComponentType>
inline ComponentType* ComponentManager::AddComponent(const EntityId entityId, const ComponentType& component)
{
    return GetComponentArray<ComponentType>().AddComponent(entityId, component);
}

template <typename ComponentType>
inline ComponentRef<ComponentType> ComponentManager::GetComponent(const EntityId entityId)
{
    return ComponentRef<ComponentType>{ entityId, &GetComponentArray<ComponentType>() };
}

template <typename T>
inline void ComponentManager::RemoveComponent(EntityId entityId)
{
    GetComponentArray<T>().RemoveComponent(entityId);
}

} // Namespace ScarlEntt
