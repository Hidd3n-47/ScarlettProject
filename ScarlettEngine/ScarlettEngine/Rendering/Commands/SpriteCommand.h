#pragma once

#include <ScarlEntt/ComponentRef.h>

#include <ScarlettGameCore/Components/Transform.h>

#include "Rendering/RenderCommand.h"

namespace Scarlett
{
//todo change the commands to use polymorphism: Seems to lose the variables when using polymorphism.
class SpriteCommand //: public RenderCommand
{
public:
    SpriteCommand(const ScarlettMath::Vec4 color, const ScarlEntt::ComponentRef<ScarlettGame::Transform> transform)
        : color(color), transform(transform)
    { /* Empty. */ }

    ScarlettMath::Vec4 color;
    ScarlEntt::ComponentRef<ScarlettGame::Transform> transform;
};


} // Namespace Scarlett.
