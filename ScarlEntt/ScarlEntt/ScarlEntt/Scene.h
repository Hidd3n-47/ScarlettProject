#pragma once

#include "ScarlEnttpch.h"

#include "ComponentManager.h"

namespace ScarlEntt {

class EntityHandle;

// Todo Christian Update the documentation of Scene when known.
/**
* The __Scene__ that all entities and components are associated with.
* The definition of a scene is still in development.
* Idea is that a world will consist of multiple __Scenes__, and each __Scene__ be a "level" with its own entities and components.
*/
class Scene
{
public:
    Scene() = default;
    ~Scene() = default;

    /**
     * Create a new _entity_.
     * @return Returns the unique ID of the entity.
     */
    EntityHandle CreateEntityHandle();
    /**
     * Create a new _entity_. 
     * @return Returns the unique ID of the entity.
     */
    EntityId CreateEntity();
    /**
     * Destroy an _entity_.
     * @return Returns the unique ID of the entity.
     */
    void DestroyEntity(EntityId& entity);

    /**
     * Get the number of entities that are currently active in the scene.
     * @return Returns the number of active/alive entities in the scene.
     */
    inline EntityId GetActiveEntitiesCount() const { return mNextFreeEntity - mDeletedEntities; }

    /**
     * Gets if the passed in _entity_ is active/alive in the scene.
     * @param entity: The ID of the entity being requested if active.
     * @return Returns the number of active entities in the scene.
     */
    inline bool IsEntityActive(const EntityId entity) const { return !(entity & InvalidEntityId); }

    /**
     * Gets the __ComponentManager__ of the _Scene_.
     * @see ComponentManger
     * @return Returns a pointer to the component manager of the scene.
     */
    inline ComponentManager* GetComponentManager() { return &mComponentManager; }
private:
    EntityId mNextFreeEntity = 0;
    EntityId mDeletedEntities = 0;

    ComponentManager mComponentManager;
};

} // Namespace ScarlEntt