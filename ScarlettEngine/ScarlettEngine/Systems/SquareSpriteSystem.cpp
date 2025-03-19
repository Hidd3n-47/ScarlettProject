#include "ScarlettEnginePch.h"
#include "SquareSpriteSystem.h"

#include <ScarlEntt/Scene.h>
#include <ScarlEntt/EntityHandle.h>

#include "Rendering/Vulkan/Mesh.h"
#include "Rendering/SpriteInfoStruct.h"

#include <ScarlettGameCore/Components/Transform.h>
#include <ScarlettGameCore/Components/SquareSprite.h>

#include "Rendering/Renderer.h"

namespace Scarlett
{

SquareSpriteSystem::SquareSpriteSystem(ScarlEntt::Scene* sceneRef, ScarlEntt::ComponentManager* componentManagerRef)
{
    mSceneRef               = sceneRef;
    mComponentManagerRef    = componentManagerRef;
}

void SquareSpriteSystem::InitSystem(ScarlEntt::ISystemProperties* properties)
{
    const SquareSpriteSystemProperties* props = reinterpret_cast<SquareSpriteSystemProperties*>(properties);

    const vector<Vertex> verts
    {
        {{ -0.5f, -0.5f} },
        {{ -0.5f,  0.5f} },
        {{  0.5f,  0.5f} },
    };
    mTriOne = new Mesh(props->device, verts);

    const vector<Vertex> verts2
    {
        {{  0.5f,  0.5f} },
        {{  0.5f, -0.5f} },
        {{ -0.5f, -0.5f} },
    };
    mTriTwo = new Mesh(props->device, verts2);
}

void SquareSpriteSystem::UpdateSystem()
{
    const VkCommandBuffer commandBuffer     = Renderer::Instance().GetCommandBuffer();
    const VkPipelineLayout pipelineLayout   = Renderer::Instance().GetPipelineLayout();

    const auto& squareSprites = mComponentManagerRef->GetComponentArray<ScarlettGame::SquareSprite>();
    const auto& entityIds = squareSprites.GetCorrespondingEntityId();
    for (ScarlEntt::ComponentId i{0}; i < squareSprites.Size(); ++i)
    {
        ScarlEntt::EntityHandle entity{entityIds[i], mSceneRef };
        const ScarlettGame::Transform* transform = entity.GetComponent<ScarlettGame::Transform>();

        const float c = glm::cos(glm::radians(transform->rotation.z));
        const float s = glm::sin(glm::radians(transform->rotation.z));
        const glm::mat4 scale{ transform->scale.x, 0.0f, 0.0f, 0.0, 0.0f, transform->scale.y, 0.0f, 0.0f, 0.0f, 0.0f, transform->scale.z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
        const glm::mat4 rotat{ c, -s, 0.0f, 0.0f, s, c, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
        glm::mat4 trans{ 1.0f };
        trans[3][0] = transform->translation.x;
        trans[3][1] = transform->translation.y;
        trans[3][2] = transform->translation.z;

        const SpriteInfoStruct info
        {
            .color = squareSprites[i].color,
            .model = trans * rotat * scale
        };

        vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SpriteInfoStruct), &info);

        mTriOne->Bind(commandBuffer);
        mTriOne->Draw(commandBuffer);

        mTriTwo->Bind(commandBuffer);
        mTriTwo->Draw(commandBuffer);
    }
}

void SquareSpriteSystem::DestroySystem()
{
    delete mTriOne;
    mTriOne = nullptr;

    delete mTriTwo;
    mTriTwo = nullptr;
}

} // Namespace Scarlett
