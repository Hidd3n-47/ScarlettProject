#pragma once

#include <Scarlentt/World.h>
#include <ScarlEntt/EntityHandle.h>

#include <ScarlettTestProject/Core/TestRegistry.h>

#include "Components.h"

class ComponentTestingWithHandles
{
public:
    inline ComponentTestingWithHandles(Scarlett::TestRegistry* testRegistry)
    {
        InitTest();

        // Register Tasks
        testRegistry->AddTestCase("Component Testing With Handles", "ComponentA_ValueChangesByReference_WithHandles", ComponentA_ValueChangesByReference_WithHandles());
        testRegistry->AddTestCase("Component Testing With Handles", "Entity2RemoveComponentWorkedCorrectly_WithHandles", Entity2RemoveComponentWorkedCorrectly_WithHandles());
        testRegistry->AddTestCase("Component Testing With Handles", "Entity1ComponentB_ValueStillCorrectAfterRemovalOfComponents_WithHandles", Entity1ComponentB_ValueStillCorrectAfterRemovalOfComponents_WithHandles());
        testRegistry->AddTestCase("Component Testing With Handles", "ComponentA_ArrayCorrect_WithHandles", ComponentA_ArrayCorrect_WithHandles());
        testRegistry->AddTestCase("Component Testing With Handles", "ComponentB_ArrayCorrect_WithHandles", ComponentB_ArrayCorrect_WithHandles());
        testRegistry->AddTestCase("Component Testing With Handles", "ComponentC_ArrayCorrect_WithHandles", ComponentC_ArrayCorrect_WithHandles());
    }

    inline void InitTest()
    {
        mScene->GetComponentManager()->RegisterComponent<ComponentA>();
        mScene->GetComponentManager()->RegisterComponent<ComponentB>();
        mScene->GetComponentManager()->RegisterComponent<ComponentC>();

        mEntity1 = mScene->CreateEntityHandle();
        mEntity2 = mScene->CreateEntityHandle();
        mEntity3 = mScene->CreateEntityHandle();

        mEntity3.AddComponent<ComponentA>()->value = 3;
        mEntity3.AddComponent<ComponentB>()->value = 3.0f;
        mEntity3.AddComponent<ComponentC>()->value = true;

        auto compA = mEntity2.AddComponent<ComponentA>();
        auto compB = mEntity2.AddComponent<ComponentB>();

        compA->value = 1;
        compB->value = 1.0f;

        mEntityComponentA = mEntity1.AddComponent<ComponentA>();
        mEntityComponentA->value = 100;
        mEntity1.AddComponent<ComponentB>()->value = 2.0f;

        // ComponentArrayA [    3,    1,  100 ]
        // Entity -> CompA [    3,    2,    1 ]

        // ComponentArrayB [ 3.0f, 1.0f, 2.0f ]
        // Entity -> CompB [    3,    2,    1 ]

        // ComponentArrayC [    T,            ]
        // Entity -> CompA [    3,            ]

        mEntity2.RemoveComponent<ComponentA>();

        mEntity3.RemoveComponent<ComponentB>();

        mEntity2.RemoveComponent<ComponentB>();


        // ComponentArrayA [    3,  100 ]
        // Entity -> CompA [    3,    1 ]

        // ComponentArrayB [       2.0f ]
        // Entity -> CompB [          1 ]

        // ComponentArrayC [          T ]
        // Entity -> CompA [          3 ]
    }


    inline bool ComponentA_ValueChangesByReference_WithHandles()
    {
        return mEntity1.GetComponent<ComponentA>()->value == 100;
    }

    inline bool Entity2RemoveComponentWorkedCorrectly_WithHandles()
    {
        return mEntity2.GetComponent<ComponentA>() == nullptr;
    }

    bool Entity1ComponentB_ValueStillCorrectAfterRemovalOfComponents_WithHandles()
    {
        return mEntity1.GetComponent<ComponentB>()->value == 2.0f;
    }

    // ComponentArrayA [    3,  100 ]
    // Entity -> CompA [    3,    1 ]
    inline bool ComponentA_ArrayCorrect_WithHandles()
    {
        bool passed = true;
        passed &= mEntity1.GetComponent<ComponentA>() != nullptr;
        passed &= mEntity2.GetComponent<ComponentA>() == nullptr;
        passed &= mEntity3.GetComponent<ComponentA>() != nullptr;

        passed &= mEntity1.GetComponent<ComponentA>()->value == 100;
        passed &= mEntity3.GetComponent<ComponentA>()->value == 3;
        return passed;
    }

    // ComponentArrayB [       2.0f ]
    // Entity -> CompB [          1 ]
    inline bool ComponentB_ArrayCorrect_WithHandles()
    {
        bool passed = true;
        passed &= mEntity1.GetComponent<ComponentB>() != nullptr;
        passed &= mEntity2.GetComponent<ComponentB>() == nullptr;
        passed &= mEntity3.GetComponent<ComponentB>() == nullptr;

        passed &= mEntity1.GetComponent<ComponentB>()->value == 2.0f;
        return passed;
    }

    // ComponentArrayC [          T ]
    // Entity -> CompA [          3 ]
    inline bool ComponentC_ArrayCorrect_WithHandles()
    {
        bool passed = true;
        passed &= mEntity1.GetComponent<ComponentC>() == nullptr;
        passed &= mEntity2.GetComponent<ComponentC>() == nullptr;
        passed &= mEntity3.GetComponent<ComponentC>() != nullptr;

        passed &= mEntity3.GetComponent<ComponentC>()->value;
        return passed;
    }

    // ComponentArrayA [    3,  100 ]
    // Entity -> CompA [    3,    1 ]

    // ComponentArrayB [       2.0f ]
    // Entity -> CompB [          1 ]

    // ComponentArrayC [          T ]
    // Entity -> CompA [          3 ]
    inline bool GetComponentsCorrect()
    {
        bool passed = true;
        {
            ComponentA* compA = nullptr;
            ComponentB* compB = nullptr;

            mEntity1.GetComponents(compA, compB);
            passed &= mScene->GetComponentManager()->GetComponent<ComponentA>(mEntity1.GetEntityId()) == compA;
            passed &= mScene->GetComponentManager()->GetComponent<ComponentB>(mEntity1.GetEntityId()) == compB;
        }

        {
            ComponentA* compA = nullptr;
            ComponentC* compC = nullptr;

            mEntity3.GetComponents(compA, compC);
            passed &= mScene->GetComponentManager()->GetComponent<ComponentA>(mEntity3.GetEntityId()) == compA;
            passed &= mScene->GetComponentManager()->GetComponent<ComponentC>(mEntity3.GetEntityId()) == compC;
        }

        {
            ScarlEntt::EntityHandle ent = mScene->CreateEntityHandle();
            ent.AddComponent<ComponentA>();
            ent.AddComponent<ComponentB>();
            ent.AddComponent<ComponentC>();

            ComponentA* compA = nullptr;
            ComponentB* compB = nullptr;
            ComponentC* compC = nullptr;
            ent.GetComponents<ComponentA, ComponentB, ComponentC>(compA, compB, compC);

            passed &= mScene->GetComponentManager()->GetComponent<ComponentA>(ent.GetEntityId()) == compA;
            passed &= mScene->GetComponentManager()->GetComponent<ComponentB>(ent.GetEntityId()) == compB;
            passed &= mScene->GetComponentManager()->GetComponent<ComponentC>(ent.GetEntityId()) == compC;
        }
        return passed;
    }

private:
    ScarlEntt::World  mWorld = ScarlEntt::World();
    ScarlEntt::Scene* mScene = mWorld.GetMainScene();
    ScarlEntt::EntityHandle mEntity1;
    ScarlEntt::EntityHandle mEntity2;
    ScarlEntt::EntityHandle mEntity3;
    ComponentA* mEntityComponentA;
};
