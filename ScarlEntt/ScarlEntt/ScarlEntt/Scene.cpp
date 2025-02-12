#include "ScarlEnttpch.h"
#include "Scene.h"

#include "EntityHandle.h"

namespace ScarlEntt
{

EntityHandle Scene::CreateEntityHandle()
{
    const EntityId entityId = CreateEntity();
    return EntityHandle{ this, entityId };
}

EntityId Scene::CreateEntity()
{
    SCARLENTT_ASSERT(mNextFreeEntity < InvalidEntityId && "Max number of entities reached.");

    return mNextFreeEntity++;
}

void Scene::DestroyEntity(EntityId& entity)
{
    SCARLENTT_ASSERT(entity < mNextFreeEntity && "Trying to destroy an entity that is not yet created.");
    SCARLENTT_ASSERT(entity < InvalidEntityId && "Passed in entity that surpasses invalid index.");
    SCARLENTT_ASSERT(IsEntityActive(entity)   && "Trying to destroy a destroyed entity.");

    ++mDeletedEntities;

    entity |= InvalidEntityId;
}

} // Namespace ScarlEntt.
