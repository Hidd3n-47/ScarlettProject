#include "ScarlettEnginepch.h"
#include "SquareSprite.h"

#include "SpriteInfoStruct.h"

namespace Scarlett
{

SquareSprite::SquareSprite(Device* device)
{
    vector<Vertex> verts
    {
        {{ -0.5f, -0.5f} },
        {{ -0.5f,  0.5f} },
        {{  0.5f,  0.5f} },
    };
    mTriOne = new Mesh(device, verts);

    vector<Vertex> verts2
    {
        {{  0.5f,  0.5f} },
        {{  0.5f, -0.5f} },
        {{ -0.5f, -0.5f} },
    };
    mTriTwo = new Mesh(device, verts2);

    mSpriteInfo.color = { 0.0f, 0.0f, 1.0f, 1.0f };
}

SquareSprite::~SquareSprite()
{
    delete mTriOne;
    delete mTriTwo;
}

void SquareSprite::Draw(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout)
{
    const float c = glm::cos(glm::radians(mRotation));
    const float s = glm::sin(glm::radians(mRotation));
    const glm::mat4 scale{ mScale.x, 0.0f, 0.0f, 0.0, 0.0f, mScale.y, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
    const glm::mat4 rotat{ c, -s, 0.0f, 0.0f, s, c, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
    glm::mat4 trans{ 1.0f };
    trans[3][0] = mPosition.x;
    trans[3][1] = mPosition.y;
    mSpriteInfo.model = trans * rotat * scale;

    vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SpriteInfoStruct), &mSpriteInfo);

    mTriOne->Bind(commandBuffer);
    mTriOne->Draw(commandBuffer);

    mTriTwo->Bind(commandBuffer);
    mTriTwo->Draw(commandBuffer);
}

} // Namespace Scarlett.