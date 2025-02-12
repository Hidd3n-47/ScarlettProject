#pragma once

#include <vulkan/vulkan_core.h>

namespace Scarlett {


struct SwapChainDetails
{
    VkSurfaceCapabilitiesKHR    surfaceCapabilities;
    vector<VkSurfaceFormatKHR>  formats;
    vector<VkPresentModeKHR>    presentationModes;
};

struct QueueFamilyIndices
{
    int graphicsFamily = -1;
    int presentationFamily = -1;

    inline bool IsValid() const { return graphicsFamily >= 0 && presentationFamily >= 0; }
};

class Window;

class Device
{
    friend class VulkanRenderer;
    friend class VulkanRendererEditor;
 public:

    Device()  = default;
    ~Device() = default;

    Device(const Device &)              = delete;
    Device(Device &&)                   = delete;
    Device& operator=(Device &&)        = delete;
    Device& operator=(const Device&)    = delete;

    void Init(const Window* windowRef);
    void Destroy();

    [[nodiscard]]uint32     FindMemoryType(const uint32_t typeFilter, const VkMemoryPropertyFlags properties) const;
    [[nodiscard]]VkFormat   FindSupportedFormat(const std::vector<VkFormat>& candidates, const VkImageTiling tiling, const VkFormatFeatureFlags features) const;

    void CreateImageWithInfo(const VkImageCreateInfo& imageInfo, const VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory) const;
    void CreateBuffer(const VkDeviceSize size, const VkBufferUsageFlags usage, const VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) const;

    [[nodiscard]] inline VkDevice           GetDevice()             const { return mDevice; }
    [[nodiscard]] inline VkSurfaceKHR       GetSurface()            const { return mSurface; }
    [[nodiscard]] inline VkCommandPool      GetCommandPool()        const { return mGraphicsCommandPool; }
    [[nodiscard]] inline VkQueue            GetPresentQueue()       const { return mPresentQueue; }
    [[nodiscard]] inline VkQueue            GetGraphicsQueue()      const { return mGraphicsQueue; }
    [[nodiscard]] inline SwapChainDetails   GetSwapChainDetails()   const { return GetSwapChainDetails(mPhysicalDevice); }
    [[nodiscard]] inline QueueFamilyIndices GetQueueFamilyIndices() const { return mQueueFamilyIndices; }
private:
    void CreateInstance();
    void CreateDebugMessenger();
    void CreateSurface();
    void ChoosePhysicalDevice();
    void CreateLogicalDevice();

    void CreateCommandPool();

    // Todo: Fix returning a vector.
    [[nodiscard]] static vector<const char*> GetRequiredExtensions();
    [[nodiscard]] static bool CheckExtensionsSupported(const vector<const char*>& extensionsToCheck);
    [[nodiscard]] static bool CheckValidationLayersSupported(const vector<const char*>& validationLayersToCheck);
    [[nodiscard]] QueueFamilyIndices GetQueueFamilyIndices(VkPhysicalDevice physicalDevice) const;
    [[nodiscard]] bool CheckDeviceSuitable(const VkPhysicalDevice device) const;
    [[nodiscard]] bool CheckDeviceExtensionSupport(const VkPhysicalDevice device) const;
    [[nodiscard]] SwapChainDetails GetSwapChainDetails(const VkPhysicalDevice device) const;

    const Window* mWindowRef;

    VkInstance                  mInstance;
    VkDebugUtilsMessengerEXT    mDebugMessenger;

    VkSurfaceKHR                mSurface;

    VkPhysicalDevice            mPhysicalDevice;
    VkDevice                    mDevice;

    QueueFamilyIndices          mQueueFamilyIndices;
    VkQueue                     mGraphicsQueue;
    VkQueue                     mPresentQueue;

    VkCommandPool               mGraphicsCommandPool;

    vector<const char *> mValidationLayers;
    vector<const char *> mDeviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
};

} // Namespace Scarlett