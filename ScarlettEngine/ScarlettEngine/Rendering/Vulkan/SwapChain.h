#pragma once

#include <vulkan/vulkan.h>

namespace Scarlett {

class Device;

class SwapChain
{
public:
    SwapChain(Device* device, const VkExtent2D extent, const SwapChain* previousSwapChain);
    ~SwapChain();

    SwapChain(const SwapChain&)             = delete;
    SwapChain(SwapChain&&)                  = delete;
    SwapChain& operator=(SwapChain&&)       = delete;
    SwapChain& operator=(const SwapChain&)  = delete;

    [[nodiscard]] VkFormat FindDepthFormat() const;

    VkResult AcquireNextImage(uint32* imageIndex) const;
    VkResult SubmitCommandBuffers(const VkCommandBuffer* buffer, const uint32* imageIndex);

    [[nodiscard]] inline uint32_t        GetWidth()                         const   { return mExtent.width; }
    [[nodiscard]] inline uint32_t        GetHeight()                        const   { return mExtent.height; }
    [[nodiscard]] inline VkRenderPass    GetRenderPass()                    const   { return mRenderPass; }
    [[nodiscard]] inline size_t          GetImageCount()                    const   { return mSwapChainImages.size(); }
    [[nodiscard]] inline VkSwapchainKHR  GetSwapChain()                     const   { return mSwapChain; }
    [[nodiscard]] inline VkExtent2D      GetSwapChainExtent()               const   { return mExtent; }
    [[nodiscard]] inline VkImageView     GetImageView(const int index)      const   { return mSwapChainImageViews[index]; }
    [[nodiscard]] inline VkFormat        GetSwapChainImageFormat()          const   { return mSwapChainImageFormat; }
    [[nodiscard]] inline VkFramebuffer   GetFrameBuffer(const int index)    const   { return mSwapChainFrameBuffers[index]; }
    [[nodiscard]] inline float           GetExtentAspectRatio()             const   { return static_cast<float>(mExtent.width) / static_cast<float>(mExtent.height); }

    static constexpr int MAX_IMAGES_PER_FRAME = 2;
private:
    void Init(const SwapChain* previousSwapChain);
    void Destroy();

    void CreateSwapChain(const SwapChain* previousSwapChain);
    void CreateImageViews();
    void CreateRenderPass();
    void CreateDepthResources();
    void CreateFrameBuffers();
    void CreateSyncObjects();

    [[nodiscard]] static VkSurfaceFormatKHR     ChooseSwapSurfaceFormat(const vector<VkSurfaceFormatKHR>& availableFormats);
    [[nodiscard]] static VkPresentModeKHR       ChooseSwapPresentMode(const vector<VkPresentModeKHR>& availablePresentModes);
    [[nodiscard]] VkExtent2D                    ChooseSwapChainExtent(const VkSurfaceCapabilitiesKHR& capabilities) const;

    Device*         mDevice;
    VkSwapchainKHR  mSwapChain;

    VkFormat         mSwapChainImageFormat;
    VkExtent2D       mExtent;
    VkExtent2D       mWindowExtent;

    vector<VkFramebuffer>   mSwapChainFrameBuffers;
    VkRenderPass            mRenderPass;

    vector<VkImage>             mDepthImages;
    vector<VkDeviceMemory>      mDepthImageMemory;
    vector<VkImageView>         mDepthImageViews;
    vector<VkImage>             mSwapChainImages;
    vector<VkImageView>         mSwapChainImageViews;

    vector<VkSemaphore>     mImageAvailableSemaphores;
    vector<VkSemaphore>     mRenderFinishedSemaphores;
    vector<VkFence>         mInFlightFences;
    vector<VkFence>         mImagesInFlight;

    size_t mCurrentFrame = 0;
};

} // Namespace Scarlett.