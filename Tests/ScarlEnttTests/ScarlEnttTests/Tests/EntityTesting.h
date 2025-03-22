#pragma once

#include <vector>

#include <ScarlEntt/EntityManager.h>

class EntityTesting
{
public:
    explicit inline EntityTesting(Scarlett::TestRegistry* testRegistry)
    {
        testRegistry->AddTestCase("Entity Testing", "RequestingFiftyEntities", RequestingFiftyEntities);
        testRegistry->AddTestCase("Entity Testing", "AddTenEntitiesRemoveFiveAddTen", AddTenEntitiesRemoveFiveAddTen);
    }

    inline static bool RequestingFiftyEntities()
    {
        constexpr int NUM_ENTITIES = 50;

        bool passed = true;
        ScarlEntt::EntityManager entityManager;

        // Create the entities.
        ScarlEntt::EntityId entities[50];
        for (ScarlEntt::EntityId& entity : entities)
        {
            entity = entityManager.CreateEntity();
        }

        // Check the generated entity ID's.
        for (ScarlEntt::EntityId i{0}; i < NUM_ENTITIES; ++i)
        {
            passed &= (entities[i] == i);
        }

        return passed;
    }

    inline static bool AddTenEntitiesRemoveFiveAddTen()
    {
        constexpr int NUM_ENTITIES          = 10;
        constexpr int NUM_ENTITIES_ADDED    = 10;
        constexpr int NUM_ENTITIES_REMOVED  = 5;

        bool passed = true;
        ScarlEntt::EntityManager entityManager;

        const ScarlEntt::EntityId expectedEntities[NUM_ENTITIES - NUM_ENTITIES_REMOVED + NUM_ENTITIES_ADDED] = {
             0,  1,  2,  3,  4,/* 5,  6,  7,  8,  9,*/
            10, 11, 12, 13, 14,  15, 16, 17, 18, 19
        };

        // Create the first 10 entities.
        std::vector<ScarlEntt::EntityId> entities;

        for (ScarlEntt::EntityId i {0}; i < NUM_ENTITIES; ++i)
        {
            entities.push_back(entityManager.CreateEntity());
        }

        // Remove 5 entities.
        for (ScarlEntt::EntityId i{0}; i < NUM_ENTITIES_REMOVED; ++i)
        {
            entityManager.DestroyEntity(entities.back());
            passed &= !ScarlEntt::EntityManager::IsAlive(entities.back());
            entities.pop_back();
        }

        // Add another 10 entities.
        for (ScarlEntt::EntityId i {0}; i < NUM_ENTITIES_ADDED; ++i)
        {
            entities.push_back(entityManager.CreateEntity());
        }

        passed &= std::size(expectedEntities) == entities.size();
        if (!passed) { return false;}

        for (ScarlEntt::EntityId i{0}; i < entities.size(); ++i)
        {
            passed &= (entities[i] == expectedEntities[i]);
        }

        return passed;
    }
};
