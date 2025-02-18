#pragma once

#include <ScarlettTestProject/Core/TestRegistry.h>

#include <ScarlEntt/World.h>
#include <ScarlEntt/EntityHandle.h>

#include "Components.h"

class ComponentTesting
{
public:
    inline ComponentTesting(Scarlett::TestRegistry* testRegistry)
    {
        InitTest();

        // Register Tests.
        testRegistry->AddTestCase("Component Testing", "ComponentA_ValueChangesByReference", ComponentA_ValueChangesByReference());
        testRegistry->AddTestCase("Component Testing", "Entity2RemoveComponentWorkedCorrectly", Entity2RemoveComponentWorkedCorrectly());
        testRegistry->AddTestCase("Component Testing", "Entity1ComponentB_ValueStillCorrectAfterRemovalOfComponents", Entity1ComponentB_ValueStillCorrectAfterRemovalOfComponents());
        testRegistry->AddTestCase("Component Testing", "ComponentA_ArrayCorrect", ComponentA_ArrayCorrect());
        testRegistry->AddTestCase("Component Testing", "ComponentB_ArrayCorrect", ComponentB_ArrayCorrect());
        testRegistry->AddTestCase("Component Testing", "ComponentC_ArrayCorrect", ComponentC_ArrayCorrect());
    }

    inline void InitTest()
    {
        mScene->GetComponentManager()->RegisterComponent<ComponentA>();
        mScene->GetComponentManager()->RegisterComponent<ComponentB>();
        mScene->GetComponentManager()->RegisterComponent<ComponentC>();

        mEntity = mScene->CreateEntity();
        mEntity2 = mScene->CreateEntity();
        mEntity3 = mScene->CreateEntity();

        mScene->GetComponentManager()->AddComponent<ComponentA>(mEntity3)->value = 3;
        mScene->GetComponentManager()->AddComponent<ComponentB>(mEntity3)->value = 3.0f;
        mScene->GetComponentManager()->AddComponent<ComponentC>(mEntity3)->value = true;

        auto compA = mScene->GetComponentManager()->AddComponent<ComponentA>(mEntity2);
        auto compB = mScene->GetComponentManager()->AddComponent<ComponentB>(mEntity2);

        compA->value = 1;
        compB->value = 1.0f;

        mEntityComponentA = mScene->GetComponentManager()->AddComponent<ComponentA>(mEntity);
        mEntityComponentA->value = 100;
        mScene->GetComponentManager()->AddComponent<ComponentB>(mEntity)->value = 2.0f;

        // ComponentArrayA [    3,    1,  100 ]
        // Entity -> CompA [    3,    2,    1 ]

        // ComponentArrayB [ 3.0f, 1.0f, 2.0f ]
        // Entity -> CompB [    3,    2,    1 ]

        // ComponentArrayC [    T,            ]
        // Entity -> CompA [    3,            ]

        mScene->GetComponentManager()->RemoveComponent<ComponentA>(mEntity2);

        mScene->GetComponentManager()->RemoveComponent<ComponentB>(mEntity3);

        mScene->GetComponentManager()->RemoveComponent<ComponentB>(mEntity2);

        // ComponentArrayA [    3,  100 ]
        // Entity -> CompA [    3,    1 ]

        // ComponentArrayB [       2.0f ]
        // Entity -> CompB [          1 ]

        // ComponentArrayC [          T ]
        // Entity -> CompA [          3 ]
    }

    inline bool ComponentA_ValueChangesByReference() const
    {
        return mScene->GetComponentManager()->GetComponent<ComponentA>(mEntity)->value == 100;
    }

    inline bool Entity2RemoveComponentWorkedCorrectly() const
    {
        return mScene->GetComponentManager()->GetComponent<ComponentA>(mEntity2) == nullptr;
    }

    bool Entity1ComponentB_ValueStillCorrectAfterRemovalOfComponents() const
    {
        return mScene->GetComponentManager()->GetComponent<ComponentB>(mEntity)->value == 2.0f;
    }

    // ComponentArrayA [    3,  100 ]
    // Entity -> CompA [    3,    1 ]
    inline bool ComponentA_ArrayCorrect() const
    {
        bool passed = true;
        passed &= mScene->GetComponentManager()->GetComponent<ComponentA>(mEntity)  != nullptr;
        passed &= mScene->GetComponentManager()->GetComponent<ComponentA>(mEntity2) == nullptr;
        passed &= mScene->GetComponentManager()->GetComponent<ComponentA>(mEntity3) != nullptr;

        passed &= mScene->GetComponentManager()->GetComponent<ComponentA>(mEntity)->value   == 100;
        passed &= mScene->GetComponentManager()->GetComponent<ComponentA>(mEntity3)->value  == 3;

        return passed;
    }

    // ComponentArrayB [       2.0f ]
    // Entity -> CompB [          1 ]
    inline bool ComponentB_ArrayCorrect() const
    {
        bool passed = true;
        passed &= mScene->GetComponentManager()->GetComponent<ComponentB>(mEntity)  != nullptr;
        passed &= mScene->GetComponentManager()->GetComponent<ComponentB>(mEntity2) == nullptr;
        passed &= mScene->GetComponentManager()->GetComponent<ComponentB>(mEntity3) == nullptr;

        passed &= mScene->GetComponentManager()->GetComponent<ComponentB>(mEntity)->value == 2.0f;

        return passed;
    }

    // ComponentArrayC [          T ]
    // Entity -> CompA [          3 ]
    inline bool ComponentC_ArrayCorrect() const
    {
        bool passed = true;
        passed &= mScene->GetComponentManager()->GetComponent<ComponentC>(mEntity)  == nullptr;
        passed &= mScene->GetComponentManager()->GetComponent<ComponentC>(mEntity2) == nullptr;
        passed &= mScene->GetComponentManager()->GetComponent<ComponentC>(mEntity3) != nullptr;

        passed &= mScene->GetComponentManager()->GetComponent<ComponentC>(mEntity3)->value;

        return passed;
    }

private:
    ScarlEntt::World  mWorld = ScarlEntt::World();
    ScarlEntt::Scene* mScene = mWorld.GetMainScene();
    EntityId mEntity;
    EntityId mEntity2;
    EntityId mEntity3;
    ComponentA* mEntityComponentA;
};
