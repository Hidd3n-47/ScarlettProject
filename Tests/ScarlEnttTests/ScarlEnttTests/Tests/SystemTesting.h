#pragma once

#include <ScarlettTestProject/Core/TestRegistry.h>

#include <ScarlEntt/Scene.h>

#include "Components.h"

class SystemChangeComponentA_Value;
class SystemLowerPriority;
class SystemHigherPriority;
class SystemWithInitialParameters;

struct SystemWithInitialParametersProperties : public ScarlEntt::ISystemProperties
{
    int value = 100;
};

class SystemTesting
{
public:
    explicit inline SystemTesting(Scarlett::TestRegistry* testRegistry)
    {
        testRegistry->AddTestCase("System Testing", "SystemUpdateWorksCorrectlyForOneTick",
                                  SystemUpdateWorksCorrectlyForOneTick);
        testRegistry->AddTestCase("System Testing", "SystemUpdateWorksCorrectlyForFiveTicks",
                                  SystemUpdateWorksCorrectlyForFiveTicks);
        testRegistry->AddTestCase("System Testing", "SystemUpdateWorksCorrectlyForOneTick_WithPriorities",
                                  SystemUpdateWorksCorrectlyForOneTick_WithPriorities);
        testRegistry->AddTestCase("System Testing", "SystemUpdateWorksCorrectlyForThreeTick_WithPriorities",
                                  SystemUpdateWorksCorrectlyForThreeTick_WithPriorities);
        testRegistry->AddTestCase("System Testing", "SystemInitializedWithProperties",
                                  SystemInitializedWithProperties);
    }

    inline static bool SystemUpdateWorksCorrectlyForOneTick()
    {
        bool passed = true;

        ScarlEntt::Scene scene;

        scene.RegisterSystem<SystemChangeComponentA_Value>();
        scene.RegisterComponent<ComponentA>();

        for (ScarlEntt::EntityId i{0}; i < 5; ++i)
        {
            ScarlEntt::EntityHandle entity = scene.CreateEntity();
            entity.AddComponent<ComponentA>(0);
        }

        scene.Update();

        auto& components = scene.GetComponentManager()->GetComponentArray<ComponentA>();

        passed &= components.Size() == 5;
        for (ScarlEntt::ComponentId i{0}; i < components.Size(); ++i)
        {
            passed &= components[i].value == 200;
        }

        return passed;
    }

    inline static bool SystemUpdateWorksCorrectlyForFiveTicks()
    {
        bool passed = true;

        ScarlEntt::Scene scene;

        scene.RegisterSystem<SystemChangeComponentA_Value>();
        scene.RegisterComponent<ComponentA>();

        for (ScarlEntt::EntityId i{0}; i < 5; ++i)
        {
            ScarlEntt::EntityHandle entity = scene.CreateEntity();
            entity.AddComponent<ComponentA>(0);
        }

        for (int i{0}; i < 5; ++i)
        {
            scene.Update();
        }

        auto& components = scene.GetComponentManager()->GetComponentArray<ComponentA>();

        passed &= components.Size() == 5;
        for (ScarlEntt::ComponentId i{0}; i < components.Size(); ++i)
        {
            passed &= components[i].value == 1000;
        }

        return passed;
    }

    inline static bool SystemUpdateWorksCorrectlyForOneTick_WithPriorities()
    {
        bool passed = true;

        ScarlEntt::Scene scene;

        scene.RegisterSystem<SystemLowerPriority>(1);
        scene.RegisterSystem<SystemHigherPriority>(uint32_t{0});
        scene.RegisterComponent<ComponentMatrix>();

        for (ScarlEntt::EntityId i{0}; i < 3; ++i)
        {
            ScarlEntt::EntityHandle entity = scene.CreateEntity();
            entity.AddComponent<ComponentMatrix>(1.0f + static_cast<float>(i), 2.0f + static_cast<float>(i), 3.0f + static_cast<float>(i), 4.0f + static_cast<float>(i));
        }

        scene.Update();

        auto& components = scene.GetComponentManager()->GetComponentArray<ComponentMatrix>();

        passed &= components.Size() == 3;

        // Entity 1.
        constexpr ComponentMatrix ent1Solution { { { 23.0f, 36.0f }, { 59.0f, 92.0f } } };
        passed &= components[0] == ent1Solution;

        // Entity 2.
        constexpr ComponentMatrix ent2Solution { { { 41.0f, 64.0f }, { 77.0f, 120.0f } } };
        passed &= components[1] == ent2Solution;

        // Entity 3.
        constexpr ComponentMatrix ent3Solution { { { 59.0f, 92.0f }, { 95.0f, 148.0f } } };
        passed &= components[2] == ent3Solution;

        return passed;
    }

    inline static bool SystemUpdateWorksCorrectlyForThreeTick_WithPriorities()
    {
        bool passed = true;

        ScarlEntt::Scene scene;

        scene.RegisterSystem<SystemLowerPriority>(1);
        scene.RegisterSystem<SystemHigherPriority>(uint32_t{0});
        scene.RegisterComponent<ComponentMatrix>();

        for (ScarlEntt::EntityId i{0}; i < 3; ++i)
        {
            ScarlEntt::EntityHandle entity = scene.CreateEntity();
            entity.AddComponent<ComponentMatrix>(1.0f + static_cast<float>(i), 2.0f + static_cast<float>(i), 3.0f + static_cast<float>(i), 4.0f + static_cast<float>(i));
        }

        for (int i{0}; i < 3; ++i)
        {
            scene.Update();
        }
        auto& components = scene.GetComponentManager()->GetComponentArray<ComponentMatrix>();

        passed &= components.Size() == 3;

        // Entity 1.
        constexpr ComponentMatrix ent1Solution { { { 9967.0f, 15564.0f }, { 25531.0f, 39868.0f } } };
        passed &= components[0] == ent1Solution;

        // Entity 2.
        constexpr ComponentMatrix ent2Solution { { { 17749.0f, 27716.0f }, { 33313.0f, 52020.0f } } };
        passed &= components[1] == ent2Solution;

        // Entity 3.
        constexpr ComponentMatrix ent3Solution { { { 25531.0f, 39868.0f }, { 41095.0f, 64172.0f } } };
        passed &= components[2] == ent3Solution;

        return passed;
    }

    inline static bool SystemInitializedWithProperties()
    {
        bool passed = true;

        ScarlEntt::Scene scene;

        SystemWithInitialParametersProperties properties{};
        scene.RegisterSystem<SystemWithInitialParameters>(&properties, 0);
        scene.RegisterComponent<ComponentA>();

        for (ScarlEntt::EntityId i{0}; i < 5; ++i)
        {
            ScarlEntt::EntityHandle entity = scene.CreateEntity();
            entity.AddComponent<ComponentA>(0);
        }

        scene.Update();

        auto& components = scene.GetComponentManager()->GetComponentArray<ComponentA>();

        for (ScarlEntt::ComponentId i{0}; i < components.Size(); ++i)
        {
            passed &= components[i].value == 100;
        }

        return passed;
    }
};

class SystemChangeComponentA_Value final : public ScarlEntt::ISystem
{
public:
    explicit inline SystemChangeComponentA_Value(ScarlEntt::Scene* sceneRef, ScarlEntt::ComponentManager* componentManagerRef)
    {
        mSceneRef = sceneRef;
        mComponentManagerRef = componentManagerRef;
    }

    inline void InitSystem(ScarlEntt::ISystemProperties*) override {  }

    inline void UpdateSystem() override
    {
        auto& componentArray = mComponentManagerRef->GetComponentArray<ComponentA>();

        for (ScarlEntt::ComponentId i{0}; i < componentArray.Size(); ++i)
        {
            componentArray[i].value += 200;
        }
    }

    inline void DestroySystem() override { }
};

class SystemLowerPriority final : public ScarlEntt::ISystem
{
public:
    explicit inline SystemLowerPriority(ScarlEntt::Scene* sceneRef, ScarlEntt::ComponentManager* componentManagerRef)
    {
        mSceneRef = sceneRef;
        mComponentManagerRef = componentManagerRef;
    }

    inline void InitSystem(ScarlEntt::ISystemProperties*) override
    {
        mMatrix = { { { 1.0f, 2.0f}, { 3.0f, 4.0f} } };
    }

    inline void UpdateSystem() override
    {
        auto& componentArray = mComponentManagerRef->GetComponentArray<ComponentMatrix>();

        for (ScarlEntt::ComponentId i{0}; i < componentArray.Size(); ++i)
        {
            componentArray[i] *= mMatrix;
        }
    }

    inline void DestroySystem() override { }

private:
    ComponentMatrix mMatrix;
};

class SystemHigherPriority final : public ScarlEntt::ISystem
{
public:
    explicit inline SystemHigherPriority(ScarlEntt::Scene* sceneRef, ScarlEntt::ComponentManager* componentManagerRef)
    {
        mSceneRef = sceneRef;
        mComponentManagerRef = componentManagerRef;
    }

    inline void InitSystem(ScarlEntt::ISystemProperties*) override
    {
        mMatrix = { { { 4.0f, 3.0f}, { 2.0f, 1.0f} } };
    }

    inline void UpdateSystem() override
    {
        auto& componentArray = mComponentManagerRef->GetComponentArray<ComponentMatrix>();

        for (ScarlEntt::ComponentId i{0}; i < componentArray.Size(); ++i)
        {
            componentArray[i] *= mMatrix;
        }
    }

    inline void DestroySystem() override { }

private:
    ComponentMatrix mMatrix;
};

class SystemWithInitialParameters final : public ScarlEntt::ISystem
{
public:
    explicit inline SystemWithInitialParameters(ScarlEntt::Scene* sceneRef, ScarlEntt::ComponentManager* componentManagerRef)
    {
        mSceneRef = sceneRef;
        mComponentManagerRef = componentManagerRef;
    }

    inline void InitSystem(ScarlEntt::ISystemProperties* properties) override
    {
        mInitialValue = reinterpret_cast<SystemWithInitialParametersProperties*>(properties)->value;
    }

    inline void UpdateSystem() override
    {
        auto& componentArray = mComponentManagerRef->GetComponentArray<ComponentA>();

        for (ScarlEntt::ComponentId i{0}; i < componentArray.Size(); ++i)
        {
            componentArray[i].value = mInitialValue;
        }
    }

    inline void DestroySystem() override { }

private:
    int mInitialValue = -1;
};
