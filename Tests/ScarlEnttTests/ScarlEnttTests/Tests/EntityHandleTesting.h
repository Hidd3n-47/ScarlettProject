#pragma once

#include <ScarlEntt/Scene.h>

#include "Components.h"

class EntityHandleTesting
{
public:
    explicit inline EntityHandleTesting(Scarlett::TestRegistry* testRegistry)
    {
        testRegistry->AddTestCase("Entity Handle Testing", "RequestingFiftyEntities", RequestingFiftyEntities);
        testRegistry->AddTestCase("Entity Handle Testing", "AddingComponentsToEntityHandles", AddingComponentsToEntityHandles);
        testRegistry->AddTestCase("Entity Handle Testing", "AddingComponentsWithArgsToEntityHandles", AddingComponentsWithArgsToEntityHandles);
        testRegistry->AddTestCase("Entity Handle Testing", "AddingComponentsByMoveSemanticsToEntityHandles", AddingComponentsByMoveSemanticsToEntityHandles);
        testRegistry->AddTestCase("Entity Handle Testing", "ComponentsUpdateByReferenceWithHandles", ComponentsUpdateByReferenceWithHandles);
        testRegistry->AddTestCase("Entity Handle Testing", "AddingAndRemovingComponentsWithHandles", AddingAndRemovingComponentsWithHandles);
    }

    inline static bool RequestingFiftyEntities()
    {
        constexpr int NUM_ENTITIES = 50;

        bool passed = true;
        ScarlEntt::Scene scene;

        // Create the entities.
        ScarlEntt::EntityHandle entities[NUM_ENTITIES];
        for (ScarlEntt::EntityHandle& entity : entities)
        {
            entity = scene.CreateEntity();
        }

        // Check the generated entity ID's.
        for (ScarlEntt::EntityId i{0}; i < NUM_ENTITIES; ++i)
        {
            // This is a 'hacky' way to get the EntityId form EntityHandle without implementing a get method.
            // Since we know the memory layout of EntityId, and that the first member variable is the id,
            // we can reinterpret cast the memory layouts to get the ID.
            passed &= (*reinterpret_cast<ScarlEntt::EntityId*>(&entities[i]) == i);
        }

        return passed;
    }

    inline static bool AddingComponentsToEntityHandles()
    {
        constexpr int NUM_ENTITIES = 5;

        bool passed = true;
        ScarlEntt::Scene scene;

        scene.RegisterComponent<ComponentA>();

        ScarlEntt::EntityHandle entities[NUM_ENTITIES];
        for (int i{0}; i < NUM_ENTITIES; ++i)
        {
            entities[i] = scene.CreateEntity();
            entities[i].AddComponent<ComponentA>()->value = i;
        }

        for (int i{0}; i < NUM_ENTITIES; ++i)
        {
            passed &= entities[i].GetComponent<ComponentA>()->value == i;
        }

        return passed;
    }

    inline static bool AddingComponentsWithArgsToEntityHandles()
    {
        constexpr int NUM_ENTITIES = 5;

        bool passed = true;
        ScarlEntt::Scene scene;

        scene.RegisterComponent<ComponentA>();

        ScarlEntt::EntityHandle entities[NUM_ENTITIES];
        for (int i{0}; i < NUM_ENTITIES; ++i)
        {
            entities[i] = scene.CreateEntity();
            (void)entities[i].AddComponent<ComponentA>(i);
        }

        for (int i{0}; i < NUM_ENTITIES; ++i)
        {
            passed &= entities[i].GetComponent<ComponentA>()->value == i;
        }

        return passed;
    }

    inline static bool AddingComponentsByMoveSemanticsToEntityHandles()
    {
        constexpr int NUM_ENTITIES = 5;

        bool passed = true;
        ScarlEntt::Scene scene;

        scene.RegisterComponent<ComponentA>();

        ScarlEntt::EntityHandle entities[NUM_ENTITIES];
        for (int i{0}; i < NUM_ENTITIES; ++i)
        {
            ComponentA component{ i };
            entities[i] = scene.CreateEntity();
            (void)entities[i].AddComponent<ComponentA>(component);
        }

        for (int i{0}; i < NUM_ENTITIES; ++i)
        {
            passed &= entities[i].GetComponent<ComponentA>()->value == i;
        }

        return passed;
    }

    inline static bool ComponentsUpdateByReferenceWithHandles()
    {
        constexpr int NUM_ENTITIES          = 5;
        constexpr int MULTIPLY_VALUE        = 10;
        constexpr int ENTITY_THREE_VALUE    = 1000;

        bool passed = true;
        ScarlEntt::Scene scene;

        scene.RegisterComponent<ComponentA>();

        ScarlEntt::EntityHandle entities[NUM_ENTITIES];
        for (int i{0}; i < NUM_ENTITIES; ++i)
        {
            entities[i] = scene.CreateEntity();
            entities[i].AddComponent<ComponentA>()->value = i;
        }

        auto* componentEntityThree = entities[3].GetComponent<ComponentA>();

        for (int i{0}; i < NUM_ENTITIES; ++i)
        {
            entities[i].GetComponent<ComponentA>()->value *= MULTIPLY_VALUE;
        }

        for (int i{0}; i < 5; ++i)
        {
            passed &= entities[i].GetComponent<ComponentA>()->value == i * MULTIPLY_VALUE;
        }

        componentEntityThree->value = ENTITY_THREE_VALUE;

        passed &= entities[3].GetComponent<ComponentA>()->value == ENTITY_THREE_VALUE;

        return passed;
    }

    inline static bool AddingAndRemovingComponentsWithHandles()
    {
        constexpr int NUM_ENTITIES          = 5;

        bool passed = true;
        ScarlEntt::Scene scene;

        scene.RegisterComponent<ComponentA>();

        ScarlEntt::EntityHandle entities[NUM_ENTITIES];
        for (int i{0}; i < NUM_ENTITIES; ++i)
        {
            entities[i] = scene.CreateEntity();
            entities[i].AddComponent<ComponentA>()->value = i;
        }

        for (int i{0}; i < NUM_ENTITIES; ++i)
        {
            passed &= entities[i].GetComponent<ComponentA>()->value == i;
        }

        for (int i{0}; i < NUM_ENTITIES; ++i)
        {
            entities[i].RemoveComponent<ComponentA>();
        }

        for (int i{0}; i < NUM_ENTITIES; ++i)
        {
            passed &= entities[i].GetComponent<ComponentA>() == nullptr;
        }

        passed &= scene.GetComponentManager()->GetComponentArray<ComponentA>().Size() == 0;

        return passed;
    }
};
