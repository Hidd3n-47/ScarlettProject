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
        ScarlEntt::Scene scene;

        scene.RegisterComponent<ComponentA>();

        auto entity = scene.CreateEntity();
        (void)entity.AddComponent<ComponentA>();

        passed &= entity.GetComponent<ComponentA>().IsValid();

        return passed;
    }

    inline static bool AddingComponentToEntityWithParametersAndRemoving()
    {
        constexpr int COMPONENT_A_VALUE = 1;

        bool passed = true;
        ScarlEntt::Scene scene;

        scene.RegisterComponent<ComponentA>();

        auto entity = scene.CreateEntity();
        (void)entity.AddComponent<ComponentA>(COMPONENT_A_VALUE);

        const ScarlEntt::ComponentRef<ComponentA> componentA = entity.GetComponent<ComponentA>();
        passed &= componentA.IsValid() && componentA->value == COMPONENT_A_VALUE;

        entity.RemoveComponent<ComponentA>();
        passed &= !entity.GetComponent<ComponentA>().IsValid();

        return passed;
    }

    inline static bool AddingComponentToEntityWithMoveSemanticsAndRemoving()
    {
        constexpr int COMPONENT_A_VALUE = 4;

        bool passed = true;
        ScarlEntt::Scene scene;

        scene.RegisterComponent<ComponentA>();

        ComponentA componentA { COMPONENT_A_VALUE };

        auto entity = scene.CreateEntity();
        (void)entity.AddComponent<ComponentA>(componentA);

        const ScarlEntt::ComponentRef<ComponentA> componentA_FromGet = entity.GetComponent<ComponentA>();
        passed &= componentA_FromGet.IsValid() && componentA_FromGet->value == COMPONENT_A_VALUE;

        entity.RemoveComponent<ComponentA>();
        passed &= !entity.GetComponent<ComponentA>().IsValid();

        return passed;
    }

    inline static bool ComponentValuesUpdateWithinScope()
    {
        constexpr int COMPONENT_A_VALUE       = 4;
        constexpr int COMPONENT_A_NEW_VALUE   = 5;

        bool passed = true;
        ScarlEntt::Scene scene;

        scene.RegisterComponent<ComponentA>();

        auto entity = scene.CreateEntity();
        (void)entity.AddComponent<ComponentA>(COMPONENT_A_VALUE);

        const ScarlEntt::ComponentRef<ComponentA> componentA_FromGet = entity.GetComponent<ComponentA>();

        componentA_FromGet->value = COMPONENT_A_NEW_VALUE;

        passed &= entity.GetComponent<ComponentA>()->value == COMPONENT_A_NEW_VALUE;

        return passed;
    }

    static constexpr int OUT_OF_SCOPE_COMPONENT_VALUE = 100;
    template <typename ComponentType>
    inline static void OutOfScopeComponentUpdate(ScarlEntt::EntityHandle entity)
    {
        entity.GetComponent<ComponentType>()->value = OUT_OF_SCOPE_COMPONENT_VALUE;
    }

    inline static bool ComponentValuesUpdateOutOfScope()
    {
        constexpr int COMPONENT_A_VALUE = 4;

        bool passed = true;
        ScarlEntt::Scene scene;

        scene.RegisterComponent<ComponentA>();

        auto entity = scene.CreateEntity();
        (void)entity.AddComponent<ComponentA>(COMPONENT_A_VALUE);

        (void)entity.GetComponent<ComponentA>();

        OutOfScopeComponentUpdate<ComponentA>(entity);

        passed &= entity.GetComponent<ComponentA>()->value == OUT_OF_SCOPE_COMPONENT_VALUE;

        return passed;
    }

    template <typename ComponentType>
    inline static void OutOfScopeComponentRemove(const ScarlEntt::EntityHandle entity)
    {
        entity.RemoveComponent<ComponentType>();
    }

    inline static bool ComponentRemoveOutOfScope()
    {
        constexpr int COMPONENT_A_VALUE = 4;

        bool passed = true;
        ScarlEntt::Scene scene;

        scene.RegisterComponent<ComponentA>();

        auto entity = scene.CreateEntity();
        (void)entity.AddComponent<ComponentA>(COMPONENT_A_VALUE);

        (void)entity.GetComponent<ComponentA>();

        OutOfScopeComponentRemove<ComponentA>(entity);

        passed &= !entity.GetComponent<ComponentA>().IsValid();

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
        ScarlEntt::Scene scene;

        scene.RegisterComponent<ComponentA>();
        scene.RegisterComponent<ComponentB>();
        scene.RegisterComponent<ComponentC>();

        auto ent1 = scene.CreateEntity();
        auto ent2 = scene.CreateEntity();
        auto ent3 = scene.CreateEntity();


        // ComponentArrayA [  100,    1,    3 ]
        // Entity -> CompA [    1,    2,    3 ]

        // ComponentArrayB [ 1.0f, 2.0f, 3.0f ]
        // Entity -> CompB [    1,    2,    3 ]

        // ComponentArrayC [    T,            ]
        // Entity -> CompA [    3,            ]
        (void)ent1.AddComponent<ComponentA>(COMPONENT_A_VALUE_ENT_1);
        (void)ent1.AddComponent<ComponentB>(COMPONENT_B_VALUE_ENT_1);

        (void)ent2.AddComponent<ComponentA>(COMPONENT_A_VALUE_ENT_2);
        (void)ent2.AddComponent<ComponentB>(COMPONENT_B_VALUE_ENT_2);

        (void)ent3.AddComponent<ComponentA>(COMPONENT_A_VALUE_ENT_3);
        (void)ent3.AddComponent<ComponentB>(COMPONENT_B_VALUE_ENT_3);
        (void)ent3.AddComponent<ComponentC>(COMPONENT_C_VALUE_ENT_3);

        ent2.RemoveComponent<ComponentA>();
        ent2.RemoveComponent<ComponentB>();
        ent3.RemoveComponent<ComponentB>();

        passed &= !ent2.GetComponent<ComponentA>().IsValid();
        passed &= !ent2.GetComponent<ComponentB>().IsValid();
        passed &= !ent3.GetComponent<ComponentB>().IsValid();


        // ComponentArrayA [  100,  3 ]
        // Entity -> CompA [    1,  3 ]

        // ComponentArrayB [     1.0f ]
        // Entity -> CompB [        1 ]

        // ComponentArrayC [        T ]
        // Entity -> CompA [        3 ]

        passed &= ent1.GetComponent<ComponentA>()->value == COMPONENT_A_VALUE_ENT_1;
        passed &= ent3.GetComponent<ComponentA>()->value == COMPONENT_A_VALUE_ENT_3;

        passed &= IsCloseTo(ent1.GetComponent<ComponentB>()->value, COMPONENT_B_VALUE_ENT_1);

        passed &= ent3.GetComponent<ComponentC>()->value == COMPONENT_C_VALUE_ENT_3;

        ScarlEntt::ComponentArray<ComponentA>& componentsA = scene.GetComponentManager()->GetComponentArray<ComponentA>();
        ScarlEntt::ComponentArray<ComponentB>& componentsB = scene.GetComponentManager()->GetComponentArray<ComponentB>();
        ScarlEntt::ComponentArray<ComponentC>& componentsC = scene.GetComponentManager()->GetComponentArray<ComponentC>();

        passed &= componentsA[0].value == COMPONENT_A_VALUE_ENT_1;
        passed &= componentsA[1].value == COMPONENT_A_VALUE_ENT_3;

        passed &= IsCloseTo(componentsB[0].value, COMPONENT_B_VALUE_ENT_1);

        passed &= componentsC[0].value == COMPONENT_C_VALUE_ENT_3;

        return passed;
    }

    inline static bool ComponentArrayIteratingAndChangingOverTenEntities()
    {
        constexpr int       NUM_ENTITIES      = 10;
        constexpr int       MULTIPLY_VALUE    = 10;

        bool passed = true;
        ScarlEntt::Scene scene;

        scene.RegisterComponent<ComponentA>();

        // Create entities and add components with ID as the value.
        ScarlEntt::EntityHandle entities [NUM_ENTITIES];
        for (int i {0}; i < NUM_ENTITIES; ++i)
        {
            entities[i] = scene.CreateEntity();
            (void)entities[i].AddComponent<ComponentA>(i);
        }

        ScarlEntt::ComponentArray<ComponentA>& componentsA = scene.GetComponentManager()->GetComponentArray<ComponentA>();

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

        entities[std::size(entities) - 1].RemoveComponent<ComponentA>();

        passed &= componentsA.Size() == 9;
        for (ScarlEntt::ComponentId i {0}; i < componentsA.Size(); ++i)
        {
            passed &= componentsA[i].value == static_cast<int>(i) * 10;
        }

        return passed;
    }
};
