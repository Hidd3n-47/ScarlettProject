#include "ScarlEnttpch.h"
#include "Scene.h"

namespace ScarlEntt
{

EntityHandle Scene::CreateEntity()
{
    const EntityId entityId = mEntityManager.CreateEntity();

#ifdef DEV_CONFIGURATION
    const EntityHandle entityHandle{ entityId, this };
    mEntities.push_back(entityHandle);
    return entityHandle;
#else // DEV_CONFIGURATION.
    return EntityHandle{ entityId, this };
#endif // Else DEV_CONFIGURATION.
}

void Scene::Update() const
{
    mSystemManager.UpdateSystems();
}

} // Namespace ScarlEntt
