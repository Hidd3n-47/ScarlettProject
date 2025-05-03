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
     * Deserialize the component based off XML node and add to the entity.
     * @param componentTypeId The type ID of the component.
     * @param node The XML node of the component.
     */
    void AddDeserializedComponent(const ComponentTypeId& componentTypeId, XmlNode* node) const;

    /**
     * Add a default component to the entity.
     * @param componentTypeId The type ID of the component.
     */
    void AddDefaultComponent(const ComponentTypeId& componentTypeId) const;

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
    * @brief Retrieve a vector of \c ComponentView for the components of the entity.
    * @note This is only available in Dev Configuration and is meant to be used for debugging.
    * @see \c ComponentView
    * @return Returns a \c ComponentView for each of the components on the entity.
    */
    [[nodiscard]] const vector<ComponentView>& GetComponents() const;
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

inline void EntityHandle::AddDeserializedComponent(const ComponentTypeId& componentTypeId, XmlNode* node) const
{
    mComponentManagerRef->AddDeserializedComponent(mEntityId, componentTypeId, node);

    const std::string TAG_TYPE_ID = "struct Scarlett::Component::Tag";
    if (componentTypeId.Type() == TAG_TYPE_ID)
    {
        AddEntityHandleToTagComponent();
    }
}

inline void EntityHandle::AddDefaultComponent(const ComponentTypeId& componentTypeId) const
{
    mComponentManagerRef->AddDefaultComponent(componentTypeId, mEntityId);
}

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
inline const vector<ComponentView>& EntityHandle::GetComponents() const
{
    return mComponentManagerRef->GetComponents(mEntityId);
}
#endif // DEV_CONFIGURATION.

} // Namespace ScarlEntt
