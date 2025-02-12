#pragma once

#include "SpriteInfoStruct.h"
#include "Vulkan/Mesh.h"

namespace Scarlett {

class Device;

class SquareSprite
{
public:
     SquareSprite(Device* device);
    ~SquareSprite();

    void Draw(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout);

    SpriteInfoStruct mSpriteInfo;
    glm::vec2 mScale = glm::vec2(1.0f);
    glm::vec2 mPosition = glm::vec2(0.0f);
    float mRotation = 0.0f;
private:

    Mesh* mTriOne;
    Mesh* mTriTwo;

};

} // Namespace Scarlett.