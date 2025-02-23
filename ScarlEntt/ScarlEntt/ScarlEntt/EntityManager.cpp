#include "ScarlEnttpch.h"
#include "EntityManager.h"

#include "ScarlEntt.h"

namespace ScarlEntt
{

EntityId EntityManager::CreateEntity()
{
    SCARLENTT_ASSERT(mAliveEntities + 1 != Entity::InvalidEntityId && "Exceeded maximum number of entities.");
    return mAliveEntities++;
}

void EntityManager::DestroyEntity(EntityId& entityId)
{
    entityId = Entity::InvalidEntityId;
    ++mNumDestroyedEntities;
}

} // Namespace ScarlEntt
