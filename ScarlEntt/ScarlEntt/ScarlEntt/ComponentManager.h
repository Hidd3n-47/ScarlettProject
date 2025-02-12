#pragma once

#include "ComponentArray.h"

// Todo Christian: Take a look at returning the actual components and not pointers to things.
namespace ScarlEntt
{

class ComponentManager
{
public:
    ComponentManager() = default;
    ~ComponentManager();

    /**
    * Register the component to the world registry.
    * __Note: All components need to be registered before they can be used.__
    */
    template <typename T>
    void RegisterComponent();

    /**
     * Get the array of a specific component type.
     * @see ComponentArray
     * @return Returns the component array of a specific component type.
     */
    template<typename T>
    ComponentArray<T>* GetComponentArray();

    /**
     * Add a component to a specific _entity_.
     * @param entityId: The entity ID for which we are adding the component to.
     */
    template <typename T>
    T* AddComponent(EntityId entityId);

    /**
     * Retrieve a pointer to a component of a specific _entity_.
     * Note that this is a raw pointer and should not be cached due to memory potentially changing on __RemoveComponent__.
     * @param entityId: The entity ID for which we are requesting the component.
     * @return Returns the __component__ if found, __nullptr__ otherwise.
     */
    template <typename T>
    T* GetComponent(EntityId entityId);

    template <typename T>
    EntityId GetEntityIdFromComponentId(ComponentId componentId);
    
    /**
     * Remove a component (if found) of a specific _entity_.
     * @param entityId: The entity ID for which we are removing the component.
     */
    template <typename T>
    void RemoveComponent(EntityId entityId);

private:
    std::unordered_map<const char*, IComponentArray*> mComponents;
};

/*
  ======================================================================================================================================================
                                                                                                                                                        */

inline ComponentManager::~ComponentManager()
{
    for (auto it : mComponents)
    {
        delete it.second;
    }
}

template <typename T>
inline void ComponentManager::RegisterComponent()
{
    // Todo (Christian): Implements asserts to ensure not registered and that it can be registered.
    // TODO (Christian): Why is SCARLENTT_ASSERT not working? it is not defining it? doesnt appear to be circular includes, just seems that SCARLENTT_DEBUG is not defined for this file ??? 
    const char* id = typeid(T).name();
    SCARLENTT_ASSERT(!mComponents.contains(id) && "Registering component type more than once.");
    assert(!mComponents.contains(id) && "Registering component type more than once.");

    mComponents[id] = new ComponentArray<T>();
}

template<typename T>
inline ComponentArray<T>* ComponentManager::GetComponentArray()
{
    const char* id = typeid(T).name();

    // TODO Christian: change assert based off comment above with SCARLENTT_ASSERT not working correctly.
    assert(mComponents.contains(id) && "Component not registered before use.");

    return static_cast<ComponentArray<T>*>(mComponents[id]);
}

template <typename T>
inline T* ComponentManager::AddComponent(EntityId entityId)
{
    return GetComponentArray<T>()->AddComponent(entityId);
}

template <typename T>
inline T* ComponentManager::GetComponent(EntityId entityId)
{
    return GetComponentArray<T>()->GetComponent(entityId);
}

template <typename T>
EntityId ComponentManager::GetEntityIdFromComponentId(ComponentId componentId)
{
    return GetComponentArray<T>()->GetEntityIdFromComponentId(componentId);
}

template <typename T>
inline void ComponentManager::RemoveComponent(EntityId entityId)
{
    GetComponentArray<T>()->RemoveComponent(entityId);
}

} // Namespace ScarlEntt.