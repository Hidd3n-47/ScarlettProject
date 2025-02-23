#pragma once

#include "EntityHandle.h"
#include "EntityManager.h"
#include "SystemManager.h"
#include "ComponentManager.h"

namespace ScarlEntt
{

/**
 * @class Scene: A scene is a collection of Entities, Components and systems.<br/>
 * Entities, Components and Systems registration, creation, updating  and destruction is all interfaced through their respective scene.<br/>
 * This means that the scene is the direct owner of all entities, components and systems and control the life-time of each.
 */
class Scene
{
public:
    Scene() = default;
    ~Scene() = default;

    Scene(const Scene&)               = delete;
    Scene(Scene&&)                    = delete;
    Scene& operator=(Scene&& )        = delete;
    Scene& operator=(const Scene& )   = delete;

    /**
    * @brief Create an entity and get a@code EntityHandle@endcode to the created entity.
    * @see EntityHandle
    * @return A handle to the created entity.
    */
    [[nodiscard]] EntityHandle CreateEntity();

    /**
    * @brief Update the scene. This updates all systems registered to the scene.
    */
    void Update() const;

    /**
    * @brief Register the component so that the component can be used with entities and systems associated with the scene.
    * @note: __All components need to be registered before they can be used.__
    */
    template <typename ComponentType>
    inline void RegisterComponent() { mComponentManager.RegisterComponent<ComponentType>(); }

    /**
    * @brief Register a system. Registering a system adds it the update queue with required priority.
    * @tparam System The class of the System that is being registered. Note that this class must inherit from@code ISystem@endcode.
    * @param priority The priority that the system is updated at. The lower the priority, the sooner it will be updated, i.e. priority 0 will be updated before priority 1.
    */
    template <typename System>
    inline void RegisterSystem(const uint32 priority = std::numeric_limits<uint32>::max()) { mSystemManager.RegisterSystem<System>(&mComponentManager, priority); }

    /**
    * @brief Get a reference to the@code ComponentManager@endcode of the scene.
    * @see ComponentManager
    * @return A reference to the component manager of the scene.
    */
    [[nodiscard]] inline ComponentManager* GetComponentManager() { return &mComponentManager; }
private:
    EntityManager       mEntityManager;
    ComponentManager    mComponentManager;
    SystemManager       mSystemManager;
};

} // Namespace ScarlEntt.
