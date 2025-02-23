#pragma once

#include <ScarlettTestProject/Core/TestRegistry.h>

#include "Components.h"
#include "ScarlEntt/ComponentManager.h"

class ComponentTesting
{
public:
    inline explicit ComponentTesting(Scarlett::TestRegistry* testRegistry)
    {
        // Register Tests.
        testRegistry->AddTestCase("Component Testing", "AddingComponentToEntity", AddingComponentToEntity);
        testRegistry->AddTestCase("Component Testing", "AddingComponentToEntityWithParametersAndRemoving", AddingComponentToEntityWithParametersAndRemoving);
        testRegistry->AddTestCase("Component Testing", "AddingComponentToEntityWithMoveSemanticsAndRemoving", AddingComponentToEntityWithMoveSemanticsAndRemoving);
        testRegistry->AddTestCase("Component Testing", "ComponentValuesUpdateWithinScope", ComponentValuesUpdateWithinScope);
        testRegistry->AddTestCase("Component Testing", "ComponentValuesUpdateOutOfScope", ComponentValuesUpdateOutOfScope);
        testRegistry->AddTestCase("Component Testing", "ComponentRemoveOutOfScope", ComponentRemoveOutOfScope);
        testRegistry->AddTestCase("Component Testing", "ComplexThreeComponentAddAndRemove_AndComponentUpdateCorrectly", ComplexThreeComponentAddAndRemove_AndComponentUpdateCorrectly);
        testRegistry->AddTestCase("ComponentTesting", "ComponentArrayIteratingAndChangingOverTenEntities", ComponentArrayIteratingAndChangingOverTenEntities);
    }

    inline static bool AddingComponentToEntity()
    {
        bool passed = true;
        ScarlEntt::EntityManager entityManager;
        ScarlEntt::ComponentManager componentManager;

        componentManager.RegisterComponent<ComponentA>();

        const auto entity = entityManager.CreateEntity();
        componentManager.AddComponent<ComponentA>(entity);

        passed &= componentManager.GetComponent<ComponentA>(entity) != nullptr;

        return passed;
    }

    inline static bool AddingComponentToEntityWithParametersAndRemoving()
    {
        bool passed = true;
        ScarlEntt::EntityManager entityManager;
        ScarlEntt::ComponentManager componentManager;

        componentManager.RegisterComponent<ComponentA>();

        const auto entity = entityManager.CreateEntity();
        componentManager.AddComponent<ComponentA>(entity, 1);

        const ComponentA* componentA = componentManager.GetComponent<ComponentA>(entity);
        passed &= componentA != nullptr && componentA->value == 1;

        componentManager.RemoveComponent<ComponentA>(entity);
        passed &= componentManager.GetComponent<ComponentA>(entity) == nullptr;

        return passed;
    }

    inline static bool AddingComponentToEntityWithMoveSemanticsAndRemoving()
    {
        bool passed = true;
        ScarlEntt::EntityManager entityManager;
        ScarlEntt::ComponentManager componentManager;

        componentManager.RegisterComponent<ComponentA>();

        ComponentA componentA { 4 };

        const auto entity = entityManager.CreateEntity();
        componentManager.AddComponent<ComponentA>(entity, componentA);

        const ComponentA* componentA_FromGet = componentManager.GetComponent<ComponentA>(entity);
        passed &= componentA_FromGet != nullptr && componentA_FromGet->value == 4;

        componentManager.RemoveComponent<ComponentA>(entity);
        passed &= componentManager.GetComponent<ComponentA>(entity) == nullptr;

        return passed;
    }

    inline static bool ComponentValuesUpdateWithinScope()
    {
        bool passed = true;
        ScarlEntt::EntityManager entityManager;
        ScarlEntt::ComponentManager componentManager;

        componentManager.RegisterComponent<ComponentA>();

        const auto entity = entityManager.CreateEntity();
        componentManager.AddComponent<ComponentA>(entity, 4);

        ComponentA* componentA_FromGet = componentManager.GetComponent<ComponentA>(entity);

        componentA_FromGet->value = 5;

        passed &= componentManager.GetComponent<ComponentA>(entity)->value == 5;

        return passed;
    }

    template <typename ComponentType>
    inline static void OutOfScopeComponentUpdate(const ScarlEntt::EntityId entityId, ScarlEntt::ComponentManager& componentManager)
    {
        componentManager.GetComponent<ComponentType>(entityId)->value = 100;
    }

    inline static bool ComponentValuesUpdateOutOfScope()
    {
        bool passed = true;
        ScarlEntt::EntityManager entityManager;
        ScarlEntt::ComponentManager componentManager;

        componentManager.RegisterComponent<ComponentA>();

        const auto entity = entityManager.CreateEntity();
        componentManager.AddComponent<ComponentA>(entity, 4);

        ComponentA* componentA_FromGet = componentManager.GetComponent<ComponentA>(entity);

        OutOfScopeComponentUpdate<ComponentA>(entity, componentManager);

        passed &= componentManager.GetComponent<ComponentA>(entity)->value == 100;

        return passed;
        }

        template <typename ComponentType>
        inline static void OutOfScopeComponentRemove(const ScarlEntt::EntityId entityId, ScarlEntt::ComponentManager& componentManager)
        {
        componentManager.RemoveComponent<ComponentType>(entityId);
    }

    inline static bool ComponentRemoveOutOfScope()
    {
        bool passed = true;
        ScarlEntt::EntityManager entityManager;
        ScarlEntt::ComponentManager componentManager;

        componentManager.RegisterComponent<ComponentA>();

        const auto entity = entityManager.CreateEntity();
        componentManager.AddComponent<ComponentA>(entity, 4);

        ComponentA* componentA_FromGet = componentManager.GetComponent<ComponentA>(entity);

        OutOfScopeComponentRemove<ComponentA>(entity, componentManager);

        passed &= componentManager.GetComponent<ComponentA>(entity) == nullptr;

        return passed;
    }

    inline static bool ComplexThreeComponentAddAndRemove_AndComponentUpdateCorrectly()
    {
        bool passed = true;
        ScarlEntt::EntityManager entityManager;
        ScarlEntt::ComponentManager componentManager;

        componentManager.RegisterComponent<ComponentA>();
        componentManager.RegisterComponent<ComponentB>();
        componentManager.RegisterComponent<ComponentC>();

        const auto ent1 = entityManager.CreateEntity();
        const auto ent2 = entityManager.CreateEntity();
        const auto ent3 = entityManager.CreateEntity();


        // ComponentArrayA [  100,    1,    3 ]
        // Entity -> CompA [    1,    2,    3 ]

        // ComponentArrayB [ 1.0f, 2.0f, 3.0f ]
        // Entity -> CompB [    1,    2,    3 ]

        // ComponentArrayC [    T,            ]
        // Entity -> CompA [    3,            ]
        componentManager.AddComponent<ComponentA>(ent1, 100);
        componentManager.AddComponent<ComponentB>(ent1, 1.0f);

        componentManager.AddComponent<ComponentA>(ent2, 1);
        componentManager.AddComponent<ComponentB>(ent2, 2.0f);

        componentManager.AddComponent<ComponentA>(ent3, 3);
        componentManager.AddComponent<ComponentB>(ent3, 3.0f);
        componentManager.AddComponent<ComponentC>(ent3, true);

        componentManager.RemoveComponent<ComponentA>(ent2);
        componentManager.RemoveComponent<ComponentB>(ent2);
        componentManager.RemoveComponent<ComponentB>(ent3);

        passed &= componentManager.GetComponent<ComponentA>(ent2) == nullptr;
        passed &= componentManager.GetComponent<ComponentB>(ent2) == nullptr;
        passed &= componentManager.GetComponent<ComponentB>(ent3) == nullptr;


        // ComponentArrayA [  100,  3 ]
        // Entity -> CompA [    1,  3 ]

        // ComponentArrayB [     1.0f ]
        // Entity -> CompB [        1 ]

        // ComponentArrayC [        T ]
        // Entity -> CompA [        3 ]

        passed &= componentManager.GetComponent<ComponentA>(ent1)->value == 100;
        passed &= componentManager.GetComponent<ComponentA>(ent3)->value == 3;

        passed &= componentManager.GetComponent<ComponentB>(ent1)->value == 1.0f;

        passed &= componentManager.GetComponent<ComponentC>(ent3)->value == true;

        ScarlEntt::ComponentArray<ComponentA>* componentsA = componentManager.GetComponentArray<ComponentA>();
        ScarlEntt::ComponentArray<ComponentB>* componentsB = componentManager.GetComponentArray<ComponentB>();
        ScarlEntt::ComponentArray<ComponentC>* componentsC = componentManager.GetComponentArray<ComponentC>();

        passed &= (*componentsA)[0].value == 100;
        passed &= (*componentsA)[1].value == 3;

        passed &= (*componentsB)[0].value == 1.0f;

        passed &= (*componentsC)[0].value == true;

        return passed;
    }

    inline static bool ComponentArrayIteratingAndChangingOverTenEntities()
    {
        bool passed = true;
        ScarlEntt::EntityManager entityManager;
        ScarlEntt::ComponentManager componentManager;

        componentManager.RegisterComponent<ComponentA>();

        // Create entities and add components with ID as the value.
        ScarlEntt::EntityId entities [10];
        for (ScarlEntt::EntityId i {0}; i < 10; ++i)
        {
            entities[i] = entityManager.CreateEntity();
            componentManager.AddComponent<ComponentA>(entities[i], static_cast<int>(i));
        }

        ScarlEntt::ComponentArray<ComponentA>* componentsA = componentManager.GetComponentArray<ComponentA>();

        // Test iterating over components.
        for (ScarlEntt::ComponentId i {0}; i < componentsA->Size(); ++i)
        {
            passed &= (*componentsA)[i].value == static_cast<int>(i);
        }

        // Iterate over components and change value by multiplying by 10.
        for (ScarlEntt::ComponentId i {0}; i < componentsA->Size(); ++i)
        {
            (*componentsA)[i].value *= 10;
        }

        // Test that components are iterated by reference.
        for (ScarlEntt::ComponentId i {0}; i < componentsA->Size(); ++i)
        {
            passed &= (*componentsA)[i].value == static_cast<int>(i) * 10;
        }

        componentManager.RemoveComponent<ComponentA>(entities[std::size(entities) - 1]);

        passed &= componentsA->Size() == 9;
        for (ScarlEntt::ComponentId i {0}; i < componentsA->Size(); ++i)
        {
            passed &= (*componentsA)[i].value == static_cast<int>(i) * 10;
        }

        return passed;
    }
};
