#include "ScarlEnttpch.h"
#include "SystemManager.h"

#include <ranges>

namespace ScarlEntt
{

SystemManager::~SystemManager()
{
    for (auto& systemArray : mSystems | std::views::values)
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
    for (const auto& systemArray : mSystems | std::views::values)
    {
        for (ISystem* system : systemArray)
        {
            system->UpdateSystem();
        }
    }
}

} // Namespace ScarlEntt
