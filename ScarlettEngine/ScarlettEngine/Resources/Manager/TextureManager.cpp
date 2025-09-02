#include "ScarlettEnginePch.h"
#include "TextureManager.h"

#include <stb_image/stb_image.h>

#include "Rendering/Vulkan/VulkanTexture.h"
#include "Rendering/Vulkan/VulkanRenderer.h"

namespace Scarlett
{

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
    for (const Texture* texture : mLoadedTextures)
    {
        delete texture;
    }
}

void TextureManager::Init(const WeakRef<Device> device)
{
    mRendererDevice = device;

    // Add constant textures for the engine.
    // -------------------------------------------------------------------------------------------------------------------
    // White texture. [This texture is first so that all textures by default are plain white].
    constexpr std::array<uint8, 4> whiteTexture = { 0xff, 0xff, 0xff, 0xff };
    mLoadedTextures.emplace_back(new VulkanTexture{ mRendererDevice, whiteTexture.data(), 1, 1 });
    // Invalid texture.
    constexpr std::array<uint8, 4> invalidTexture = { 0xff, 0xff, 0x0, 0xff };
    mLoadedTextures.emplace_back(new VulkanTexture{ mRendererDevice, invalidTexture.data(), 1, 1 });

    (void)AddTexture(Filepath{ "Assets/Textures/TextureUV.png" });
    (void)AddTexture(Filepath{ "Assets/Textures/viking_room.png" });
    // -------------------------------------------------------------------------------------------------------------------
}

uint32 TextureManager::AddTexture(const Filepath& filepath)
{
    if (!filepath.DoesPathExist())
    {
        SCARLETT_ELOG("Failed to create texture since path does not exist: '{0}'", filepath.GetAbsolutePath());
        return INVALID_TEXTURE_INDEX;
    }

    // Check if the texture at the given filepath is already loaded, and if so, return that ID.
    if (mFilepathToTextureId.contains(filepath))
    {
        return mFilepathToTextureId[filepath];
    }

    stbi_set_flip_vertically_on_load(true);

    constexpr int requiredChannels = 4;

    int x, y, n;
    const uint8* buffer = stbi_load(filepath.GetAbsolutePath().c_str(), &x, &y, &n, requiredChannels);

    if (buffer == nullptr || x < 0 || y < 0 || n < 0)
    {
        SCARLETT_ELOG("Failed to create texture at path: '{0}'", filepath.GetAbsolutePath());
        return INVALID_TEXTURE_INDEX;
    }

    mLoadedTextures.emplace_back(new VulkanTexture{ mRendererDevice, buffer, static_cast<uint32>(x), static_cast<uint32>(y) });

    const uint32 index = static_cast<uint32>(mLoadedTextures.size()) - 1;
    mFilepathToTextureId[filepath] = index;

    stbi_image_free((void*)buffer);

    return index;
}

} // Namespace Scarlett.