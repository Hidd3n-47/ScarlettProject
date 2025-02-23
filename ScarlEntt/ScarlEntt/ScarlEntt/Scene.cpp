#include "ScarlEnttpch.h"
#include "Scene.h"

namespace ScarlEntt
{

EntityHandle Scene::CreateEntity()
{
    const EntityId entityId = mEntityManager.CreateEntity();

    return EntityHandle{ entityId, this };
}

void Scene::Update() const
{
    mSystemManager.UpdateSystems();
}

} // Namespace ScarlEntt
