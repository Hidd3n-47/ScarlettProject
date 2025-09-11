#pragma once

#include <vulkan/vulkan.h>

namespace Scarlett
{

class Device;
class Texture;

/**
 * @class TextureManager: A class used to manage textures for runtime.
 */
class TextureManager
{
friend class VulkanRenderer;
friend class MaterialManager;
public:
    TextureManager()    = default;
    ~TextureManager()   = default;

    TextureManager(const TextureManager&)               = delete;
    TextureManager(TextureManager&&)                    = delete;
    TextureManager& operator=(TextureManager&&)         = delete;
    TextureManager& operator=(const TextureManager&)    = delete;

    /**
     * @brief Initialize the texture manager.
     * @param device The graphics device used for rendering.
     */
    void Init(const WeakRef<Device> device);

    /**
     * @brief Destroy the texture manager, freeing any resources allocated.
     */
    void Destroy();

    /**
     * @brief Load a texture from disk and add it to the texture manager.
     * @param filepath The filepath of the texture that is being loaded in.
     * @param updateDescriptorSets When adding textures, descriptor sets should be updated. Added as a parameter to prevent updating when mass adding textures.
     * Defaulted to true and should remain \c true for the most part.
     * @return The \c textureId of the added texture.
     */
    [[nodiscard]] textureId AddTexture(const Filepath& filepath, const bool updateDescriptorSets = true);

    // Static texture ID's that are initialised on TextureManager creation.
    static constexpr textureId WHITE_TEXTURE_INDEX     = 0;
    static constexpr textureId INVALID_TEXTURE_INDEX   = 1;
private:
    WeakRef<Device> mRendererDevice;

    vector<Texture*> mLoadedTextures;

    std::unordered_map<Filepath, textureId> mFilepathToTextureId;

    VkDescriptorPool        mDescriptorSetPool;
    VkDescriptorSetLayout   mDescriptorSetLayout;
    VkDescriptorSet         mDescriptorSet;

    /**
     * @brief A function used to update the descriptor sets. Descriptor sets should be updated every time a new texture is added.
     */
    void UpdateDescriptorSets(const uint32 startingSlot, const uint32 count);
};

} // Namespace Scarlett.
