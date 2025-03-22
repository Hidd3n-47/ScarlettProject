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
        constexpr int COMPONENT_A_VALUE = 1;

        bool passed = true;
        ScarlEntt::EntityManager entityManager;
        ScarlEntt::ComponentManager componentManager;

        componentManager.RegisterComponent<ComponentA>();

        const auto entity = entityManager.CreateEntity();
        componentManager.AddComponent<ComponentA>(entity, COMPONENT_A_VALUE);

        const ComponentA* componentA = componentManager.GetComponent<ComponentA>(entity);
        passed &= componentA != nullptr && componentA->value == COMPONENT_A_VALUE;

        componentManager.RemoveComponent<ComponentA>(entity);
        passed &= componentManager.GetComponent<ComponentA>(entity) == nullptr;

        return passed;
    }

    inline static bool AddingComponentToEntityWithMoveSemanticsAndRemoving()
    {
        constexpr int COMPONENT_A_VALUE = 4;

        bool passed = true;
        ScarlEntt::EntityManager entityManager;
        ScarlEntt::ComponentManager componentManager;

        componentManager.RegisterComponent<ComponentA>();

        ComponentA componentA { COMPONENT_A_VALUE };

        const auto entity = entityManager.CreateEntity();
        componentManager.AddComponent<ComponentA>(entity, componentA);

        const ComponentA* componentA_FromGet = componentManager.GetComponent<ComponentA>(entity);
        passed &= componentA_FromGet != nullptr && componentA_FromGet->value == COMPONENT_A_VALUE;

        componentManager.RemoveComponent<ComponentA>(entity);
        passed &= componentManager.GetComponent<ComponentA>(entity) == nullptr;

        return passed;
    }

    inline static bool ComponentValuesUpdateWithinScope()
    {
        constexpr int COMPONENT_A_VALUE       = 4;
        constexpr int COMPONENT_A_NEW_VALUE   = 5;

        bool passed = true;
        ScarlEntt::EntityManager entityManager;
        ScarlEntt::ComponentManager componentManager;

        componentManager.RegisterComponent<ComponentA>();

        const auto entity = entityManager.CreateEntity();
        componentManager.AddComponent<ComponentA>(entity, COMPONENT_A_VALUE);

        ComponentA* componentA_FromGet = componentManager.GetComponent<ComponentA>(entity);

        componentA_FromGet->value = COMPONENT_A_NEW_VALUE;

        passed &= componentManager.GetComponent<ComponentA>(entity)->value == COMPONENT_A_NEW_VALUE;

        return passed;
    }

    static constexpr int OUT_OF_SCOPE_COMPONENT_VALUE = 100;
    template <typename ComponentType>
    inline static void OutOfScopeComponentUpdate(const ScarlEntt::EntityId entityId, ScarlEntt::ComponentManager& componentManager)
    {
        componentManager.GetComponent<ComponentType>(entityId)->value = OUT_OF_SCOPE_COMPONENT_VALUE;
    }

    inline static bool ComponentValuesUpdateOutOfScope()
    {
        constexpr int COMPONENT_A_VALUE = 4;

        bool passed = true;
        ScarlEntt::EntityManager entityManager;
        ScarlEntt::ComponentManager componentManager;

        componentManager.RegisterComponent<ComponentA>();

        const auto entity = entityManager.CreateEntity();
        componentManager.AddComponent<ComponentA>(entity, COMPONENT_A_VALUE);

        (void)componentManager.GetComponent<ComponentA>(entity);

        OutOfScopeComponentUpdate<ComponentA>(entity, componentManager);

        passed &= componentManager.GetComponent<ComponentA>(entity)->value == OUT_OF_SCOPE_COMPONENT_VALUE;

        return passed;
    }

    template <typename ComponentType>
    inline static void OutOfScopeComponentRemove(const ScarlEntt::EntityId entityId, ScarlEntt::ComponentManager& componentManager)
    {
        componentManager.RemoveComponent<ComponentType>(entityId);
    }

    inline static bool ComponentRemoveOutOfScope()
    {
        constexpr int COMPONENT_A_VALUE = 4;

        bool passed = true;
        ScarlEntt::EntityManager entityManager;
        ScarlEntt::ComponentManager componentManager;

        componentManager.RegisterComponent<ComponentA>();

        const auto entity = entityManager.CreateEntity();
        componentManager.AddComponent<ComponentA>(entity, COMPONENT_A_VALUE);

        (void)componentManager.GetComponent<ComponentA>(entity);

        OutOfScopeComponentRemove<ComponentA>(entity, componentManager);

        passed &= componentManager.GetComponent<ComponentA>(entity) == nullptr;

        return passed;
    }

    inline static bool IsCloseTo(const float a, const float b)
    {
        constexpr float EPSILON = 0.000001f;

        return fabs(a - b) < EPSILON;
    }

    inline static bool ComplexThreeComponentAddAndRemove_AndComponentUpdateCorrectly()
    {
        constexpr int       COMPONENT_A_VALUE_ENT_1     = 100;
        constexpr float     COMPONENT_B_VALUE_ENT_1     = 1.0f;

        constexpr int       COMPONENT_A_VALUE_ENT_2     = 1;
        constexpr float     COMPONENT_B_VALUE_ENT_2     = 2.0f;

        constexpr int       COMPONENT_A_VALUE_ENT_3     = 3;
        constexpr float     COMPONENT_B_VALUE_ENT_3     = 3.0f;
        constexpr bool      COMPONENT_C_VALUE_ENT_3     = true;

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
        componentManager.AddComponent<ComponentA>(ent1, COMPONENT_A_VALUE_ENT_1);
        componentManager.AddComponent<ComponentB>(ent1, COMPONENT_B_VALUE_ENT_1);

        componentManager.AddComponent<ComponentA>(ent2, COMPONENT_A_VALUE_ENT_2);
        componentManager.AddComponent<ComponentB>(ent2, COMPONENT_B_VALUE_ENT_2);

        componentManager.AddComponent<ComponentA>(ent3, COMPONENT_A_VALUE_ENT_3);
        componentManager.AddComponent<ComponentB>(ent3, COMPONENT_B_VALUE_ENT_3);
        componentManager.AddComponent<ComponentC>(ent3, COMPONENT_C_VALUE_ENT_3);

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

        passed &= componentManager.GetComponent<ComponentA>(ent1)->value == COMPONENT_A_VALUE_ENT_1;
        passed &= componentManager.GetComponent<ComponentA>(ent3)->value == COMPONENT_A_VALUE_ENT_3;

        passed &= IsCloseTo(componentManager.GetComponent<ComponentB>(ent1)->value, COMPONENT_B_VALUE_ENT_1);

        passed &= componentManager.GetComponent<ComponentC>(ent3)->value == COMPONENT_C_VALUE_ENT_3;

        ScarlEntt::ComponentArray<ComponentA>& componentsA = componentManager.GetComponentArray<ComponentA>();
        ScarlEntt::ComponentArray<ComponentB>& componentsB = componentManager.GetComponentArray<ComponentB>();
        ScarlEntt::ComponentArray<ComponentC>& componentsC = componentManager.GetComponentArray<ComponentC>();

        passed &= componentsA[0].value == COMPONENT_A_VALUE_ENT_1;
        passed &= componentsA[1].value == COMPONENT_A_VALUE_ENT_3;

        passed &= IsCloseTo(componentsB[0].value, COMPONENT_B_VALUE_ENT_1);

        passed &= componentsC[0].value == COMPONENT_C_VALUE_ENT_3;

        return passed;
    }

    inline static bool ComponentArrayIteratingAndChangingOverTenEntities()
    {
        constexpr int       NUM_ENTITIES    = 10;
        constexpr int     MULTIPLY_VALUE    = 10;

        bool passed = true;
        ScarlEntt::EntityManager entityManager;
        ScarlEntt::ComponentManager componentManager;

        componentManager.RegisterComponent<ComponentA>();

        // Create entities and add components with ID as the value.
        ScarlEntt::EntityId entities [NUM_ENTITIES];
        for (ScarlEntt::EntityId i {0}; i < NUM_ENTITIES; ++i)
        {
            entities[i] = entityManager.CreateEntity();
            componentManager.AddComponent<ComponentA>(entities[i], static_cast<int>(i));
        }

        ScarlEntt::ComponentArray<ComponentA>& componentsA = componentManager.GetComponentArray<ComponentA>();

        // Test iterating over components.
        for (ScarlEntt::ComponentId i {0}; i < componentsA.Size(); ++i)
        {
            passed &= componentsA[i].value == static_cast<int>(i);
        }

        // Iterate over components and change value by multiplying by 10.
        for (ScarlEntt::ComponentId i {0}; i < componentsA.Size(); ++i)
        {
            componentsA[i].value *= MULTIPLY_VALUE;
        }

        // Test that components are iterated by reference.
        for (ScarlEntt::ComponentId i {0}; i < componentsA.Size(); ++i)
        {
            passed &= componentsA[i].value == static_cast<int>(i) * MULTIPLY_VALUE;
        }

        componentManager.RemoveComponent<ComponentA>(entities[std::size(entities) - 1]);

        passed &= componentsA.Size() == 9;
        for (ScarlEntt::ComponentId i {0}; i < componentsA.Size(); ++i)
        {
            passed &= componentsA[i].value == static_cast<int>(i) * 10;
        }

        return passed;
    }
};
