#pragma once

#include <unordered_set>

#include "Scene.h"

namespace ScarlEntt {

// Todo Christian Considering renaming Entity Handle to just Entity.
class EntityHandle
{
public:
    // Todo Christian: Ensure that default constructed EntityHandle holds invalid values.
    EntityHandle(Scene* scene, EntityId entityId);
    EntityHandle() = default;
    ~EntityHandle() = default;

    template<typename T>
    T* AddComponent();

    template<typename T>
    T* GetComponent();

    // Todo Christian need to figure out how to get this to work with templates.
    template <typename T, typename ...Args>
    void GetComponents(T*& firstComponent, Args*& ...nComponent);

    template <typename T>
    void RemoveComponent();

    inline EntityId GetEntityId() const { return mEntityId; }
private:
    EntityId mEntityId;
    Scene* mScene;
    ComponentManager* mComponentManager;

    std::unordered_set<const char*> mComponents;

    template<typename T>
    void GetComponent_Internal(T*& component);

    inline void GetComponents() { }
};

inline EntityHandle::EntityHandle(Scene* scene, EntityId entityId)
    : mEntityId { entityId }
    , mScene { scene }
    , mComponentManager{ mScene->GetComponentManager() }
{
    // Empty.
}

template<typename T>
inline T* EntityHandle::AddComponent()
{
    const char* componentType = typeid(T).name();
    // Todo Christian change to SCARLENTT_ASSERT.
    // Todo remove check here as we don't want to have to remake the set each time we make the handle.
    //assert(!mComponents.contains(componentType) && "Failed to add component to entity as it already has this component type.");

    //mComponents.insert(componentType);

    return mComponentManager->AddComponent<T>(mEntityId);
}

template<typename T>
inline T* EntityHandle::GetComponent()
{
    return mComponentManager->GetComponent<T>(mEntityId);
}

template <typename T, typename ...Args>
inline void EntityHandle::GetComponents(T*& firstComponent, Args*& ...nComponent)
{
    firstComponent = GetComponent<T>();
    GetComponents(nComponent...);

    //std::initializer_list<int>{ (GetComponent_Internal(nComponent), 0)... };
}

template<typename T>
inline void EntityHandle::GetComponent_Internal(T*& component)
{
    component = GetComponent<T>();
}

template<typename T>
inline void EntityHandle::RemoveComponent()
{
    const char* componentType = typeid(T).name();
    // Todo Christian change to SCARLENTT_ASSERT.
    //assert(mComponents.contains(componentType) && "Failed to remove component from entity as it does not have this component type.");
    // Todo remove check here as we don't want to have to remake the set each time we make the handle.


    mComponents.erase(componentType);

    mComponentManager->RemoveComponent<T>(mEntityId);
}

} // Namespace ScarlEntt.