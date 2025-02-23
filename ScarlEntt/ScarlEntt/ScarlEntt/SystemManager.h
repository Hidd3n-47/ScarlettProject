#pragma once

#include <map>

#include "ISystem.h"

namespace ScarlEntt
{

/**
 * @class SystemManager: The manager of all systems needed for the _ECS_.<br/>
 * All systems should be registered before they are used, this creates, initializes and adds them to an Update queue.<br/>
 * Systems can be registered with different priorities, allowing the user to customise when Systems should be Updated.<br/>
 * Any registered system must inherit from the base@code ISystem@endcode class.
 */
class SystemManager
{
public:
    SystemManager() = default;
    ~SystemManager();

    SystemManager(const SystemManager&)             = delete;
    SystemManager(SystemManager&&)                  = delete;
    SystemManager& operator=(SystemManager&&)       = delete;
    SystemManager& operator=(const SystemManager&)  = delete;

    /**
     * @brief Update the systems that have been registered in order of their priority.
     * @note The lower the priority, the sooner they will be updated, i.e. priority 0 will be updated before priority 1.
     */
    void UpdateSystems() const;

    /**
     * @brief Register a system. Registering a system adds it the update queue with required priority.
     * @tparam System The class of the System that is being registered. Note that this class must inherit from@code ISystem@endcode.
     * @param componentManagerRef A pointer (reference) for the component array. This is so that the system can get the@code ComponentArray@endcode of interested components.
     * @param priority The priority that the system is updated at. The lower the priority, the sooner it will be updated, i.e. priority 0 will be updated before priority 1.
     */
    template <typename System>
    void RegisterSystem(ComponentManager* componentManagerRef, const uint32 priority);
private:
    std::map<uint32, vector<ISystem*>> mSystems;
};

/*
  ======================================================================================================================================================
                                                                                                                                                        */

template <typename System>
inline void SystemManager::RegisterSystem(ComponentManager* componentManagerRef, const uint32 priority)
{
    mSystems[priority].emplace_back(new System(componentManagerRef));
    mSystems[priority].back()->InitSystem();
}

} // Namespace ScarlEntt.
