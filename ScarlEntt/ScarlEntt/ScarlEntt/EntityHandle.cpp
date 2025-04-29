#include "ScarlEnttpch.h"
#include "EntityHandle.h"

#include "Scene.h"

#include "Components/Tag.h"

namespace ScarlEntt
{

EntityHandle::EntityHandle()
    : mEntityId{ Entity::InvalidEntityId }
    , mSceneRef{ nullptr }
    , mComponentManagerRef{ nullptr }
{
    // Empty.
}

EntityHandle::EntityHandle(const EntityId entityId, Scene* sceneRef)
    : mEntityId{ entityId }
    , mSceneRef{ sceneRef }
    , mComponentManagerRef{ mSceneRef->GetComponentManager() }
{
    // Empty.
}

void EntityHandle::AddEntityHandleToTagComponent() const
{
    GetComponent<Scarlett::Component::Tag>()->entity = EntityHandle{ mEntityId, mSceneRef };
}

} // Namespace ScarlEntt.
