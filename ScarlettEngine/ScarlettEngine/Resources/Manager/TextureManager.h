#pragma once

#include "Rendering/Texture.h"

namespace Scarlett
{
class Device;

class TextureManager
{
friend class VulkanRenderer;
friend class MaterialManager;
public:
    TextureManager();
    ~TextureManager();

    TextureManager(const TextureManager&)               = delete;
    TextureManager(TextureManager&&)                    = delete;
    TextureManager& operator=(TextureManager&&)         = delete;
    TextureManager& operator=(const TextureManager&)    = delete;

    void Init(const WeakRef<Device> device);

    [[nodiscard]] uint32 AddTexture(const Filepath& filepath);

    // Static texture ID's that are initialised on TextureManager creation.
    static constexpr uint32 WHITE_TEXTURE_INDEX     = 0;
    static constexpr uint32 INVALID_TEXTURE_INDEX   = 1;
private:
    WeakRef<Device> mRendererDevice;

    vector<Texture*> mLoadedTextures;

    std::unordered_map<Filepath, uint32> mFilepathToTextureId;
};

} // Namespace Scarlett.
