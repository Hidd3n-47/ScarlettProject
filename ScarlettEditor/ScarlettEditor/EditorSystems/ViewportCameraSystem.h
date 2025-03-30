#pragma once

#include <ScarlEntt/ISystem.h>

#include "Views/Editor/ViewportCamera.h"

namespace ScarlettEditor
{

class ViewportCameraSystem final : public ScarlEntt::ISystem
{
public:
    ViewportCameraSystem(ScarlEntt::Scene* sceneRef, ScarlEntt::ComponentManager* componentManagerRef);

    void InitSystem(ScarlEntt::ISystemProperties* properties = nullptr) override { }
    void UpdateSystem()     override;
    void DestroySystem()    override { }
private:

};


} // Namespace ScarlettEditor.
