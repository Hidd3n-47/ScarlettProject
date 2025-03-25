#include "ScarlettEnginePch.h"
#include "SquareSpriteSystem.h"

#include <ScarlEntt/Scene.h>
#include <ScarlEntt/EntityHandle.h>

#include "Rendering/Vulkan/Mesh.h"
#include "Rendering/SpriteInfoStruct.h"

#include <ScarlettGameCore/Components/Camera.h>
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
    const ScarlettGame::Camera* camera      = Renderer::Instance().GetRenderCamera();

    const auto& squareSprites = mComponentManagerRef->GetComponentArray<ScarlettGame::SquareSprite>();
    const auto& entityIds = squareSprites.GetCorrespondingEntityId();
    for (ScarlEntt::ComponentId i{0}; i < squareSprites.Size(); ++i)
    {
        ScarlEntt::EntityHandle entity{entityIds[i], mSceneRef };
        const ScarlettGame::Transform* transform = entity.GetComponent<ScarlettGame::Transform>();

        const ScarlettMath::Mat4 scale{ transform->scale.x, 0.0f, 0.0f, 0.0, 0.0f, transform->scale.y, 0.0f, 0.0f, 0.0f, 0.0f, transform->scale.z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };

        const float cX = ScarlettMath::Cos(ScarlettMath::Radians(transform->rotation.x));
        const float sX = ScarlettMath::Sin(ScarlettMath::Radians(transform->rotation.x));

        const float cY = ScarlettMath::Cos(ScarlettMath::Radians(transform->rotation.y));
        const float sY = ScarlettMath::Sin(ScarlettMath::Radians(transform->rotation.y));

        const float cZ = ScarlettMath::Cos(ScarlettMath::Radians(transform->rotation.z));
        const float sZ = ScarlettMath::Sin(ScarlettMath::Radians(transform->rotation.z));

        const ScarlettMath::Mat4 rotateX{ 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, cX, -sX, 0.0f, 0.0f, sX, cX, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
        const ScarlettMath::Mat4 rotateY{ cY, 0.0f, sY, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -sY, 0.0f, cY, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
        const ScarlettMath::Mat4 rotateZ{ cZ, -sZ, 0.0f, 0.0f, sZ, cZ, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };

        ScarlettMath::Mat4 trans{ 1.0f };
        trans[3][0] = transform->translation.x;
        trans[3][1] = transform->translation.y;
        trans[3][2] = transform->translation.z;

        ScarlettMath::Mat4 modelMatrix = { 1.0f };
        modelMatrix = glm::scale(modelMatrix, transform->scale);
        modelMatrix = glm::rotate(modelMatrix, ScarlettMath::Radians(transform->rotation.x), { 1.0f, 0.0f, 0.0f });
        modelMatrix = glm::rotate(modelMatrix, ScarlettMath::Radians(transform->rotation.y), { 0.0f, 1.0f, 0.0f });
        modelMatrix = glm::rotate(modelMatrix, ScarlettMath::Radians(transform->rotation.z), { 0.0f, 0.0f, 1.0f });
        modelMatrix = glm::translate(modelMatrix, transform->translation);

        const SpriteInfoStruct info
        {
            .color = squareSprites[i].color,
            .view  = camera->viewMatrix,
            .proj  = camera->projectionMatrix,
            .model = trans * rotateZ * rotateY * rotateX * scale
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
