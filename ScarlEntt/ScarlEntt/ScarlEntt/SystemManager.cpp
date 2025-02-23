#include "ScarlEnttpch.h"
#include "SystemManager.h"

namespace ScarlEntt
{

SystemManager::~SystemManager()
{
    for (auto& [priority, systemArray] : mSystems)
    {
        for (ISystem* system : systemArray)
        {
            system->DestroySystem();
            delete system;
        }
    }
}

void ScarlEntt::SystemManager::UpdateSystems() const
{
    for (auto& [priority, systemArray] : mSystems)
    {
        for (ISystem* system : systemArray)
        {
            system->UpdateSystem();
        }
    }
}

} // Namespace ScarlEntt
