#pragma once

#include "ComponentRef.h"
#include "ComponentManager.h"

#include "Debug.h"

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
    EntityHandle(EntityHandle&& other)                      = default;
    EntityHandle& operator=(const EntityHandle& other)      = default;
    EntityHandle& operator=(EntityHandle&& other)           = default;

    /**
    * @brief Add a component to the entity
    * @tparam ComponentType The class of the Component
    * @tparam Args Arguments that are passed to the ComponentType constructor to construct a component with initial values.
    * @param args The arguments used to initialise the component.
    * @return A reference to the created component.
    */
    template <typename ComponentType, typename ...Args>
    [[maybe_unused]] ComponentType* AddComponent(Args ...args) const;

    /**
    * @brief Add the passed in component to the entity.<br/>
    * Note: The ownership of the __component__ is passed to the ComponentManager once called, therefore, cannot be used after adding to Entity.
    * @tparam ComponentType The class of the Component.
    * @param component The component that is being added to the component array. Ownership of this component is passed over to ComponentManger after function call.
    * @return A reference to the created component.
    */
    template <typename ComponentType>
    [[maybe_unused]] ComponentType* AddComponent(const ComponentType& component) const;

    /**
    * @brief Deserialize the component from XML and add it to the entity.
    * @param node: The XML of the component to be deserialized.
    */
    void AddComponentFromXml(const ScarlettUtils::XmlElement* node) const;

    /**
    * @breif Remove a component (if found) from the _entity_.
    */
    template <typename ComponentType>
    void RemoveComponent() const;

    /**
    * @brief Retrieve a reference to the component of the entity.
    * @see \c ComponentRef
    * @return Returns a component reference of the requested component.
    */
    template <typename ComponentType>
    [[nodiscard]] ComponentRef<ComponentType> GetComponent();

    /**
    * @brief Retrieve a const reference to the component of the entity.
    * @see \c ComponentRef
    * @return Returns a const component reference of the requested component.
    */
    template <typename ComponentType>
    [[nodiscard]] ComponentRef<ComponentType> GetComponent() const;

#ifdef DEV_CONFIGURATION
    /**
    * @brief Get a vector of the components attached to the entity.
    * @see ComponentView
    * @returns A vector of \c ComponentView for the components on the entity.
    */
    [[nodiscard]] vector<ComponentView>* GetComponents() const;
#endif // DEV_CONFIGURATION.

    /**
    * @brief Get the ID of the entity.
    * @return Returns the ID of the entity.
    */
    [[nodiscard]] EntityId GetId() const { return mEntityId; }
private:
    EntityId            mEntityId;
    Scene*              mSceneRef;
    ComponentManager*   mComponentManagerRef;

    void AddEntityHandleToTagComponent() const;
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

inline void EntityHandle::AddComponentFromXml(const ScarlettUtils::XmlElement* node) const
{
    mComponentManagerRef->AddComponentFromXml(mEntityId, node);
}

template <typename ComponentType>
inline void EntityHandle::RemoveComponent() const
{
    return mComponentManagerRef->RemoveComponent<ComponentType>(mEntityId);
}

template <typename ComponentType>
inline ComponentRef<ComponentType> EntityHandle::GetComponent()
{
    return mComponentManagerRef->GetComponent<ComponentType>(mEntityId);
}

template <typename ComponentType>
inline ComponentRef<ComponentType> EntityHandle::GetComponent() const
{
    return mComponentManagerRef->GetComponent<ComponentType>(mEntityId);
}

#ifdef DEV_CONFIGURATION

inline vector<ComponentView>* EntityHandle::GetComponents() const
{
    return mComponentManagerRef->GetComponents(mEntityId);
}

#endif // DEV_CONFIGURATION.

} // Namespace ScarlEntt
