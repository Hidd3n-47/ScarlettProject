#pragma once

#include "EntityManager.h"

namespace ScarlEntt
{

/**
 * @class IComponentArray: Interface for component arrays.
 */
class IComponentArray
{
public:
    IComponentArray()               = default;
    virtual ~IComponentArray()      = default;

    IComponentArray(const IComponentArray&)               = delete;
    IComponentArray(IComponentArray&&)                    = delete;
    IComponentArray& operator=(IComponentArray&&)         = delete;
    IComponentArray& operator=(const IComponentArray&)    = delete;
};

/**
 * @class ComponentArray: The array of components to ensure that components are in contiguous memory.
 */
template <typename ComponentType>
class ComponentArray final : public IComponentArray
{
public:
    ComponentArray()            = default;
    ~ComponentArray() override  = default;

    ComponentArray(const ComponentArray&)               = delete;
    ComponentArray(ComponentArray&&)                    = delete;
    ComponentArray& operator=(ComponentArray&&)         = delete;
    ComponentArray& operator=(const ComponentArray&)    = delete;

    /**
     * @brief Add a component to an entity.
     * @tparam Args Arguments that are passed to the ComponentType constructor to construct a component with initial values.
     * @param entityId The ID of the entity that the component is being attached to.
     * @param args The arguments used to initialize the component.
     * @return A reference to the created component.
     */
    template <typename ... Args>
    ComponentType* AddComponent(const EntityId entityId, Args&& ...args);

    /**
     * @brief Add a component to an entity. The ownership of the passed in component is passed to the ComponentManager.
     * @param entityId The entity ID that the component is being attached to.
     * @param component The component that is being added to the entity.
     * @return A reference to the added component.
     */
    ComponentType* AddComponent(const EntityId entityId, const ComponentType& component);

    /**
    * @brief Removes a _component_ from the __ComponentArray__ from the _entity_.
    * @param entityId: The ID of the entity the component is being removed from.
    */
    void RemoveComponent(const EntityId entityId);

    /**
    * @brief Gets the specific component, if any, that has been allocated to the _entity_
    * @param entityId: The ID of the entity the component has been requested for.
    * @return Returns the _component_ allocated to the entity, nullptr if it has not been allocated.
    */
    ComponentType* GetComponent(const EntityId entityId);

    // Todo Christian: Find a better way to do this.
    [[nodiscard]] const vector<EntityId>& GetCorrespondingEntityId() const { return mCorrespondingEntityId;}

    /**
     * Get the size of the components in the component array.<br/>
     * The size is the active number of components.
     * @return The number of components in the component array.
     */
    [[nodiscard]] ComponentId Size() const { return static_cast<ComponentId>(mComponentArray.size()); }

    /**
     * @brief Access the __component__ at the passed in index.
     * @param index: The index of the component requested for.
     * @return The component at the passed in index.
     */
    ComponentType& operator[](ComponentId index);
    /**
     * @brief Access the __component__ at the passed in index.
     * @param index: The index of the component requested for.
     * @return The component at the passed in index.
     */
    const ComponentType& operator[](ComponentId index) const;
private:
    vector<ComponentType>   mComponentArray;
    vector<EntityId>        mCorrespondingEntityId;

    std::unordered_map<EntityId, ComponentId> mEntityToComponentMap;
    std::unordered_map<ComponentId, EntityId> mComponentToEntityMap;
};

/*
  ======================================================================================================================================================
                                                                                                                                                        */

// Todo Christian Make this return a component ref as this could be stale if a
// vector resize occurs after adding/getting the component.
template <typename ComponentType>
template <typename ... Args>
inline ComponentType* ComponentArray<ComponentType>::AddComponent(const EntityId entityId, Args&& ...args)
{
    assert(EntityManager::IsAlive(entityId) && "Entity is invalid and hence cannot add a component to."); // todo change assert.

    const ComponentId componentId = static_cast<ComponentId>(mComponentArray.size());

    mEntityToComponentMap[entityId] = componentId;
    mComponentToEntityMap[componentId] = entityId;

    mComponentArray.emplace_back(ComponentType{args...});
    mCorrespondingEntityId.emplace_back(entityId);
    return &mComponentArray.back();
}

template <typename ComponentType>
inline ComponentType* ComponentArray<ComponentType>::AddComponent(const EntityId entityId, const ComponentType& component)
{
    assert(EntityManager::IsAlive(entityId) && "Entity is invalid and hence cannot add a component to."); // todo change assert.

    const ComponentId componentId = static_cast<ComponentId>(mComponentArray.size());

    mEntityToComponentMap[entityId] = componentId;
    mComponentToEntityMap[componentId] = entityId;

    mCorrespondingEntityId.emplace_back(entityId);
    return &mComponentArray.emplace_back(std::move(component));
}

template <typename ComponentType>
inline void ComponentArray<ComponentType>::RemoveComponent(const EntityId entityId)
{
    assert(EntityManager::IsAlive(entityId) && "Entity is invalid and hence cannot remove a component from."); // todo change assert.

    const ComponentId lastComponentId       = static_cast<ComponentId>(mComponentArray.size()) - 1;
    const EntityId lastComponentEntity      = mComponentToEntityMap[lastComponentId];
    const ComponentId removedComponentId    = mEntityToComponentMap[entityId];

    mComponentArray[removedComponentId]         = std::move(mComponentArray.back());
    mCorrespondingEntityId[removedComponentId]  = mCorrespondingEntityId.back();
    mEntityToComponentMap[lastComponentEntity]  = removedComponentId;
    mComponentToEntityMap[removedComponentId]   = lastComponentEntity;

    mEntityToComponentMap.erase(entityId);
    mComponentArray.pop_back();
    mCorrespondingEntityId.pop_back();
}

template <typename ComponentType>
inline ComponentType* ComponentArray<ComponentType>::GetComponent(const EntityId entityId)
{
    assert(EntityManager::IsAlive(entityId) && "Entity is invalid and hence cannot retrieve a component."); // todo change assert.

    if (!mEntityToComponentMap.contains(entityId))
    {
        return nullptr;
    }

    return &mComponentArray[mEntityToComponentMap[entityId]];
}

template <typename ComponentType>
inline ComponentType& ComponentArray<ComponentType>::operator[](ComponentId index)
{
    // Todo Christian: Add checks on index.

    return mComponentArray[index];
}

template <typename ComponentType>
inline const ComponentType& ComponentArray<ComponentType>::operator[](ComponentId index) const
{
    // Todo Christian: Add checks on index.

    return mComponentArray[index];
}

} // Namespace ScarlEntt
