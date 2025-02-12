#pragma once
#include "ScarlEnttpch.h"

#include "Scene.h"

namespace ScarlEntt
{
// Todo Christian Update the documentation of World when known.
/**
* The __World__ that all entities and components are assigned to.
* The definition of a world is still in development.
* Idea is that a __World__ will consist of multiple scenes, and each scene be a "level" with its own entities and components.
*/
class World
{
public:
    World() = default;
    ~World() = default;

    // Todo Christian: Think of architecture with main scene. Are scenes needed? How will world contain scenes?
    inline Scene* GetMainScene() { return &mMainScene; }

private:
    Scene mMainScene;
};

} // Namespace ScalEntt.