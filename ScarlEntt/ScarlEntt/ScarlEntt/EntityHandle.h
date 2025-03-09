#pragma once
#include "ComponentManager.h"

namespace ScarlEntt
{

class Scene;

class EntityHandle
{
public:
    EntityHandle();
    explicit EntityHandle(const EntityId entityId, Scene* sceneRef);
    ~EntityHandle() = default;

    EntityHandle(const EntityHandle& other)                 = default;
    EntityHandle(EntityHandle&& other)                      = delete;
    EntityHandle& operator=(const EntityHandle& other)      = default;
    EntityHandle& operator=(EntityHandle&& other)           = delete;

    /**
    * @brief Add a component to the entity
    * @tparam ComponentType The class of the Component
    * @tparam Args Arguments that are passed to the ComponentType constructor to construct a component with initial values.
    * @param args The arguments used to initialize the component.
    * @return A reference to the created component.
    */
    template <typename ComponentType, typename ...Args>
    [[maybe_unused]] ComponentType* AddComponent(Args ...args) const;
    /**
    * @brief Add the passed in component to the entity.<br/>
    * Note: The ownership of the __component__ is passed to the ComponentManager once called, therefore cannot be used after adding to Entity.
    * @tparam ComponentType The class of the Component.
    * @param component The component that is being added to the component array. Ownership of this component is passed over to ComponentManger after function call.
    * @return A reference to the created component.
    */
    template <typename ComponentType>
    [[maybe_unused]] ComponentType* AddComponent(const ComponentType& component) const;

    /**
    * @breif Remove a component (if found) from the _entity_.
    */
    template <typename ComponentType>
    void RemoveComponent() const;

    /**
    * @brief Retrieve a pointer to the component of the entity.
    * @note This is a raw pointer and should not be cached due to memory potentially changing.
    * @return Returns the __component__ if found, __nullptr__ otherwise.
    */
    template <typename ComponentType>
    [[nodiscard]] ComponentType* GetComponent();

private:
    EntityId            mEntityId;
    Scene*              mSceneRef;
    ComponentManager*   mComponentManagerRef;
};

/*
  ======================================================================================================================================================
                                                                                                                                                        */

template <typename ComponentType, typename ...Args>
inline ComponentType* EntityHandle::AddComponent(Args ...args) const
{
    return mComponentManagerRef->AddComponent<ComponentType>(mEntityId, std::forward<Args>(args)...);
}

template <typename ComponentType>
inline ComponentType* EntityHandle::AddComponent(const ComponentType& component) const
{
    return mComponentManagerRef->AddComponent<ComponentType>(mEntityId, component);
}

template <typename ComponentType>
inline void EntityHandle::RemoveComponent() const
{
    return mComponentManagerRef->RemoveComponent<ComponentType>(mEntityId);
}

template <typename ComponentType>
inline ComponentType* EntityHandle::GetComponent()
{
    return mComponentManagerRef->GetComponent<ComponentType>(mEntityId);
}

} // Namespace ScarlEntt
