#pragma once

#include <complex.h>
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
        bool passed = true;
        ScarlEntt::Scene scene;

        // Create the entities.
        ScarlEntt::EntityHandle entities[50];
        for (ScarlEntt::EntityHandle& entity : entities)
        {
            entity = scene.CreateEntity();
        }

        // Check the generated entity ID's.
        for (ScarlEntt::EntityId i{0}; i < 50; ++i)
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
        bool passed = true;
        ScarlEntt::Scene scene;

        scene.RegisterComponent<ComponentA>();

        ScarlEntt::EntityHandle entities[5];
        for (int i{0}; i < 5; ++i)
        {
            entities[i] = scene.CreateEntity();
            entities[i].AddComponent<ComponentA>()->value = i;
        }

        for (int i{0}; i < 5; ++i)
        {
            passed &= entities[i].GetComponent<ComponentA>()->value == i;
        }

        return passed;
    }

    inline static bool AddingComponentsWithArgsToEntityHandles()
    {
        bool passed = true;
        ScarlEntt::Scene scene;

        scene.RegisterComponent<ComponentA>();

        ScarlEntt::EntityHandle entities[5];
        for (int i{0}; i < 5; ++i)
        {
            entities[i] = scene.CreateEntity();
            entities[i].AddComponent<ComponentA>(i);
        }

        for (int i{0}; i < 5; ++i)
        {
            passed &= entities[i].GetComponent<ComponentA>()->value == i;
        }

        return passed;
    }

    inline static bool AddingComponentsByMoveSemanticsToEntityHandles()
    {
        bool passed = true;
        ScarlEntt::Scene scene;

        scene.RegisterComponent<ComponentA>();

        ScarlEntt::EntityHandle entities[5];
        for (int i{0}; i < 5; ++i)
        {
            ComponentA component{ i };
            entities[i] = scene.CreateEntity();
            entities[i].AddComponent<ComponentA>(component);
        }

        for (int i{0}; i < 5; ++i)
        {
            passed &= entities[i].GetComponent<ComponentA>()->value == i;
        }

        return passed;
    }

    inline static bool ComponentsUpdateByReferenceWithHandles()
    {
        bool passed = true;
        ScarlEntt::Scene scene;

        scene.RegisterComponent<ComponentA>();

        ScarlEntt::EntityHandle entities[5];
        for (int i{0}; i < 5; ++i)
        {
            entities[i] = scene.CreateEntity();
            entities[i].AddComponent<ComponentA>()->value = i;
        }

        auto* componentEntityThree = entities[3].GetComponent<ComponentA>();

        for (int i{0}; i < 5; ++i)
        {
            entities[i].GetComponent<ComponentA>()->value *= 10;
        }

        for (int i{0}; i < 5; ++i)
        {
            passed &= entities[i].GetComponent<ComponentA>()->value == i * 10;
        }

        componentEntityThree->value = 1000;

        passed &= entities[3].GetComponent<ComponentA>()->value == 1000;

        return passed;
    }

    inline static bool AddingAndRemovingComponentsWithHandles()
    {
        bool passed = true;
        ScarlEntt::Scene scene;

        scene.RegisterComponent<ComponentA>();

        ScarlEntt::EntityHandle entities[5];
        for (int i{0}; i < 5; ++i)
        {
            entities[i] = scene.CreateEntity();
            entities[i].AddComponent<ComponentA>()->value = i;
        }

        for (int i{0}; i < 5; ++i)
        {
            passed &= entities[i].GetComponent<ComponentA>()->value == i;
        }

        for (int i{0}; i < 5; ++i)
        {
            entities[i].RemoveComponent<ComponentA>();
        }

        for (int i{0}; i < 5; ++i)
        {
            passed &= entities[i].GetComponent<ComponentA>() == nullptr;
        }

        passed &= scene.GetComponentManager()->GetComponentArray<ComponentA>().Size() == 0;

        return passed;
    }
};
