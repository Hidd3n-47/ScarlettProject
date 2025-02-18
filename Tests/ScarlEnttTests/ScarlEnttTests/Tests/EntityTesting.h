#pragma once

#include <vector>
#include <Scarlentt/World.h>

class EntityTesting
{
public:
    inline EntityTesting(Scarlett::TestRegistry* testRegistry)
    {
        InitTest();

        // Register tests.
        testRegistry->AddTestCase("Entity Testing", "Requesting50Entities", Requesting50Entities());
        testRegistry->AddTestCase("Entity Testing", "LastEntityIdIs49", LastEntityIdIs49());
        testRegistry->AddTestCase("Entity Testing", "DestroyingEntity10And20And30Worked", DestroyingEntity10And20And30Worked());
        testRegistry->AddTestCase("Entity Testing", "DestroyingEntity10And20And30Worked", DestroyingEntity10And20And30Worked());
        testRegistry->AddTestCase("Entity Testing", "TestRandomEntitiesId", TestRandomEntitiesId());
    }

    inline void InitTest()
    {
        // Create 50 entities.
        for (EntityId i = 0; i < 50; ++i)
        {
            mEntities.push_back(mScene->CreateEntity());
        }

        // Destroy entity 10, 20 and 30.
        mScene->DestroyEntity(mEntities[10]);
        mScene->DestroyEntity(mEntities[20]);
        mScene->DestroyEntity(mEntities[30]);
    }

    inline bool Requesting50Entities()const
    {
        return mEntities.size() == 50;
    }

    inline bool LastEntityIdIs49() const
    {
        return mEntities.back() == 49;
    }

    inline bool DestroyingEntity10And20And30Worked() const
    {
        bool passed = true;
        passed &= mEntities[10] == (InvalidEntityId | static_cast<EntityId>(10));
        passed &= mEntities[20] == (InvalidEntityId | static_cast<EntityId>(20));
        passed &= mEntities[30] == (InvalidEntityId | static_cast<EntityId>(30));

        passed &= !mScene->IsEntityActive(mEntities[10]);
        passed &= !mScene->IsEntityActive(mEntities[20]);
        passed &= !mScene->IsEntityActive(mEntities[30]);
        return passed;
    }

    inline bool TestRandomEntitiesAreLiving() const
    {
        bool passed = true;
        passed &= mScene->IsEntityActive(mEntities[ 5]);

        passed &= mScene->IsEntityActive(mEntities[ 9]);
        passed &= mScene->IsEntityActive(mEntities[11]);

        passed &= mScene->IsEntityActive(mEntities[15]);

        passed &= mScene->IsEntityActive(mEntities[19]);
        passed &= mScene->IsEntityActive(mEntities[21]);

        passed &= mScene->IsEntityActive(mEntities[25]);

        passed &= mScene->IsEntityActive(mEntities[29]);
        passed &= mScene->IsEntityActive(mEntities[31]);

        passed &= mScene->IsEntityActive(mEntities[35]);

        passed &= mScene->IsEntityActive(mEntities[45]);

        passed &= mScene->IsEntityActive(mEntities[49]);
        return passed;
    }

    inline bool TestRandomEntitiesId() const
    {
        bool passed = true;
        passed &= mEntities[ 5] == 5;

        passed &= mEntities[ 9] == 9;
        passed &= mEntities[11] == 11;

        passed &= mEntities[15] == 15;

        passed &= mEntities[19] == 19;
        passed &= mEntities[21] == 21;

        passed &= mEntities[25] == 25;

        passed &= mEntities[29] == 29;
        passed &= mEntities[31] == 31;

        passed &= mEntities[35] == 35;

        passed &= mEntities[45] == 45;

        passed &= mEntities[49] == 49;
        return passed;
    }

private:
    ScarlEntt::World  mWorld = ScarlEntt::World();
    ScarlEntt::Scene* mScene = mWorld.GetMainScene();
    std::vector<EntityId> mEntities;
};
