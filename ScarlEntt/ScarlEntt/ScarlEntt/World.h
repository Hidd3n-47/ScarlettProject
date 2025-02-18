#pragma once
#include "ScarlEnttpch.h"

#include "Scene.h"

namespace ScarlEntt
{
// Todo Christian Update the documentation of World when known.
/**
* The __World__ that all mEntities and components are assigned to.
* The definition of a mWorld is still in development.
* Idea is that a __World__ will consist of multiple scenes, and each mScene be a "level" with its own mEntities and components.
*/
class World
{
public:
    World() = default;
    ~World() = default;

    // Todo Christian: Think of architecture with main mScene. Are scenes needed? How will mWorld contain scenes?
    inline Scene* GetMainScene() { return &mMainScene; }

private:
    Scene mMainScene;
};

} // Namespace ScalEntt.