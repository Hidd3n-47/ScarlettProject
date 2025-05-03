#include "GameCore.h"

#include <ScarlettLogger/Log.h>

#include <Components/Tag.h>
#include <Components/Line.h>
#include <Components/Camera.h>
#include <Components/Transform.h>
#include <Components/BoundingBox.h>
#include <Components/SquareSprite.h>

namespace ScarlettGame
{

GameCore* GameCore::mInstance = new GameCore();

GameCore::~GameCore()
{
    delete mCurrentScene;
}

void GameCore::Init()
{
    Scarlett::Log::Init(true, "ScarlettGameCore");

    mCurrentScene = new ScarlEntt::Scene();

    mCurrentScene->RegisterComponent<Scarlett::Component::Tag>();
    mCurrentScene->RegisterComponent<Scarlett::Component::BoundingBox>();

    mCurrentScene->RegisterComponent<Scarlett::Component::Transform>();
    mCurrentScene->RegisterComponent<Scarlett::Component::SquareSprite>();

    mCurrentScene->RegisterComponent<Scarlett::Component::Camera>();
    mCurrentScene->RegisterComponent<Scarlett::Component::Line>();

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

    (void)ent.AddComponent<Scarlett::Component::Tag>("Entity " + std::to_string(ent.GetId()), ent);
    (void)ent.AddComponent<Scarlett::Component::BoundingBox>();

    (void)ent.AddComponent<Scarlett::Component::Transform>();

    return ent;
}

} // Namespace ScarlettGame.
