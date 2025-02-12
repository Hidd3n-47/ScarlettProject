#pragma once
#include "ScarlEnttpch.h"

namespace ScarlEntt
{

/**
 * Interface for component arrays.
 */
class IComponentArray
{
public:
    virtual ~IComponentArray() = default;
};

/**
 * The array of components to ensure that components are in contiguous memory.
 */
template <typename Component>
class ComponentArray : public IComponentArray
{
public:
    ComponentArray();
    ~ComponentArray() override;

    /**
     * Allocate a _component_ from the __ComponentArray__ to the _entity_.
     * @param entityId: The ID of the entity the component is being allocated to.
     * @return Returns the _component_ that has been allocated to the _entity_
     */
    Component* AddComponent(EntityId entityId);

    /**
     * Gets the specific component, if any, that has been allocated to the _entity_
     * @param entityId: The ID of the entity the component has been requested for.
     * @return Returns the _component_ allocated to the entity, nullptr if it has not been allocated.
     */
    Component* GetComponent(EntityId entityId);

    /**
     * Deallocates a _component_ from the __ComponentArray__ from the _entity_.
     * @param entityId: The ID of the entity the component is being deallocated from.
     */
    void RemoveComponent(EntityId entityId);

    /**
     * Gets the count of _components_ from the __ComponentArray__ that are currently allocated and active.
     * @return The number of components from the component array that are currently allocated and active.
     */
    inline ComponentId GetComponentCount() const { return mNextFreeIndex; }

    //Todo Christian: include a way to use iterator for component array instead of directly accessing each element by index.
    /**
     * Access the __component__ at the passed in index.
     * @param index: The index of the component requested for.
     * @return The component at the passed in index.
     */
    Component* operator[](ComponentId index) const;

    // Todo Christian: Do we need this sort of iterator for the array.
    // Does this break cache efficiency?
    class iterator
    {
    public:
        inline iterator(Component* array, ComponentId index, ComponentId count)
            : mArray(array)
            , mIndex(index)
            , mCount(count)
        {
            // Empty.
        }

        inline Component* value() { return &mArray[mIndex]; }
        inline ComponentId index() const { return mIndex; }
        
        inline bool operator==(const iterator& other) const
        {
            return mArray == other.mArray && mIndex == other.mIndex && mCount == other.mCount;
        }

        inline iterator& operator++()
        {
            ++mIndex;
            return *this;
        }
    private:
        Component* mArray;
        ComponentId mIndex;
        ComponentId mCount;
    };
    inline iterator begin() const
    {
        return iterator(mComponents, 0, mNextFreeIndex);
    }
    inline iterator end() const
    {
        return iterator(mComponents, mNextFreeIndex, mNextFreeIndex);
    }
private:
    Component* mComponents;
    ComponentId mNextFreeIndex = 0;

    std::unordered_map<EntityId, ComponentId> mEntityToComponentMap;
    std::unordered_map<ComponentId, EntityId> mComponentToEntityMap;

    static const int MAX_COMPONENTS = 1000;
};

/*
  ======================================================================================================================================================
                                                                                                                                                        */

template <typename Component>
inline ComponentArray<Component>::ComponentArray()
    : mComponents{ new Component[MAX_COMPONENTS] }
{
    // Empty.
}

template <typename Component>
inline ComponentArray<Component>::~ComponentArray()
{
    delete[] mComponents;
}

template <typename Component>
inline Component* ComponentArray<Component>::AddComponent(EntityId entityId)
{
    // TODO Christian Assert here if errors.

    mEntityToComponentMap[entityId] = mNextFreeIndex;

    mComponentToEntityMap[mNextFreeIndex] = entityId;

    return &mComponents[mNextFreeIndex++];
}

template <typename Component>
inline Component* ComponentArray<Component>::GetComponent(EntityId entityId)
{
    // TODO Christian Assert here if errors.
    if (!mEntityToComponentMap.contains(entityId))
    {
        return nullptr;
    }

    return &mComponents[mEntityToComponentMap[entityId]];
}

template <typename Component>
inline void ComponentArray<Component>::RemoveComponent(EntityId entityId)
{
    // TODO (Christian): Implement asserts.

    const ComponentId lastComponent = mNextFreeIndex - 1;
    const EntityId lastComponentEntity = mComponentToEntityMap[lastComponent];
    const ComponentId removedComponentId = mEntityToComponentMap[entityId];

    mComponents[removedComponentId] = mComponents[lastComponent];
    mEntityToComponentMap[lastComponentEntity] = removedComponentId;
    mComponentToEntityMap[removedComponentId] = lastComponentEntity;

    mEntityToComponentMap.erase(entityId);

    --mNextFreeIndex;
}

template <typename Component>
inline Component* ComponentArray<Component>::operator[](ComponentId index) const
{
    // Todo Christian: Add checks on index.

    return &mComponents[index];
}

} // Namespace ScarlEntt.