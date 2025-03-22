#include "GameCore.h"

#include <ScarlettLogger/Log.h>

#include "ScarlettGameCore/Components/Tag.h"
#include "ScarlettGameCore/Components/Transform.h"
#include "ScarlettGameCore/Components/SquareSprite.h"

namespace ScarlettGame
{

GameCore* GameCore::mInstance = new GameCore();

void GameCore::Init()
{
    Scarlett::Log::Init(true, "ScarlettGameCore");

    mCurrentScene = std::make_unique<ScarlEntt::Scene>();

    mCurrentScene->RegisterComponent<Tag>();
    mCurrentScene->RegisterComponent<Transform>();
    mCurrentScene->RegisterComponent<SquareSprite>();

    Scarlett::Log::GetLogger()->info("Scarlett Game Core initialized");
}

void GameCore::Destroy()
{
    delete mInstance;
    mInstance = nullptr;

    Scarlett::Log::GetLogger()->info("Scarlett Game Core destroyed");
}

ScarlEntt::EntityHandle GameCore::CreateEntity() const
{
    const ScarlEntt::EntityHandle ent = mCurrentScene->CreateEntity();

    (void)ent.AddComponent<Transform>();
    (void)ent.AddComponent<Tag>(ScarlettStl::String{ "Entity " + std::to_string(ent.GetId()) }, ent);
    (void)ent.AddComponent<SquareSprite>();

    return ent;
}

} // Namespace ScarlettGame.
