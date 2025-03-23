#pragma once

#include <memory>

#include <ScarlEntt/Scene.h>

#include "ScarlettGameCore/Src/ScarlettGame.h"

namespace ScarlettGame
{

class SCARLETT_GAME_CORE_API GameCore
{
public:
    GameCore() = default;
    ~GameCore();

    GameCore(const GameCore&)               = delete;
    GameCore(GameCore&&)                    = delete;
    GameCore& operator=(GameCore&&)         = delete;
    GameCore& operator=(const GameCore&)    = delete;

    static inline GameCore& Instance() { return *mInstance; }

    void Init();
    static void Destroy();

    ScarlEntt::EntityHandle CreateEntity() const;

    [[nodiscard]] ScarlEntt::Scene* GetActiveScene() const { return mCurrentScene; }
private:

    static GameCore* mInstance;

    ScarlEntt::Scene* mCurrentScene = nullptr;
};

} // Namespace ScarlettGame.
