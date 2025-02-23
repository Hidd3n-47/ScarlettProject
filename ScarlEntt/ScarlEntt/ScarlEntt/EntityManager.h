#pragma once

#include "Types.h"

namespace ScarlEntt
{

class EntityManager
{
public:
    EntityManager() = default;
    ~EntityManager() = default;

    EntityManager(const EntityManager&)               = delete;
    EntityManager(EntityManager&&)                    = delete;
    EntityManager& operator=(EntityManager&&)         = delete;
    EntityManager& operator=(const EntityManager&)    = delete;

    /**
     * @brief Create an entity.
     * @return The ID of the created Entity.
     */
    EntityId CreateEntity();

    /**
     * @brief Destroys the passed in entity.
     * @param entityId The id of the entity that should be destroyed.
     */
    void DestroyEntity(EntityId& entityId);

    /**
     * @brief Get the number of alive entities.
     * @return The number of alive entities.
     */
    [[nodiscard]] inline EntityId GetAliveEntities() const { return mAliveEntities; }

    /**
     * @brief Check if an entity is valid/alive.
     * @param entityId The entity ID being checked if it's valid.
     * @return @code true@endcode if entity is valid,@code false@endcode otherwise.
     */
    [[nodiscard]] inline static bool IsAlive(const EntityId entityId) { return entityId != Entity::InvalidEntityId; }
private:
    EntityId mAliveEntities             = 0;
    EntityId mNumDestroyedEntities      = 0;
};

} // Namespace ScarlEntt.
