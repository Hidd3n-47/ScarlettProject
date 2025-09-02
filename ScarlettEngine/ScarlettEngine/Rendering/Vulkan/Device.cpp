#include "ScarlettEnginepch.h"
#include "Device.h"

#include <glfw/glfw3.h>

#include "Core/Window/Window.h"
#include "Rendering/Vulkan/VulkanUtils.h"

namespace Scarlett {

namespace
{

static VkBool32 DebugCallback(
    const VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    const VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData)
{
    SCARLETT_DLOG("================================================================================================");
    switch (messageSeverity)
    {
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
        SCARLETT_ELOG("Validation Layer Info: ");
        SCARLETT_ELOG(pCallbackData->pMessage);
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
        SCARLETT_WLOG("Validation Layer Warning: ");
        SCARLETT_WLOG(pCallbackData->pMessage);
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
        SCARLETT_FLOG("Validation Layer Error: ");
        SCARLETT_FLOG(pCallbackData->pMessage);
        break;
    default:
        // Not traced at the minute.
        break;
    }

    switch (messageType)
    {
    case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
        SCARLETT_DLOG("General validation issue: Some event has happened that is unrelated to the specification or performance");
        break;
    case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
        SCARLETT_DLOG("Debug validation issue: Something has happened that violates the specification or indicates a possible mistake");
        break;
    case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
        SCARLETT_DLOG("Performance validation issue: General validation issue: Potential non-optimal use of Vulkan");
        break;
    default:
        break;
    }
    SCARLETT_DLOG("================================================================================================");

    return VK_FALSE;
}

} // Anonymous Namespace.

static VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
{
    const auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr)
    {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    }

    return VK_ERROR_EXTENSION_NOT_PRESENT;
}

static void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
{
    const auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr)
    {
        func(instance, debugMessenger, pAllocator);
    }
}

void Device::Init(const Window* windowRef)
{
    mWindowRef = windowRef;

    CreateInstance();
    CreateDebugMessenger();
    CreateSurface();
    ChoosePhysicalDevice();
    CreateLogicalDevice();
    CreateCommandPool();
}

void Device::Destroy() const
{
    vkDestroyCommandPool(mDevice, mGraphicsCommandPool, nullptr);

    vkDestroyDevice(mDevice, nullptr);
    vkDestroySurfaceKHR(mInstance, mSurface, nullptr);

#ifdef SCARLETT_DEBUG
    DestroyDebugUtilsMessengerEXT(mInstance, mDebugMessenger, nullptr);
#endif // SCARLETT_DEBUG.

    vkDestroyInstance(mInstance, nullptr);
}

uint32 Device::FindMemoryType(const uint32_t typeFilter, const VkMemoryPropertyFlags properties) const
{
    VkPhysicalDeviceMemoryProperties memProperties;

    vkGetPhysicalDeviceMemoryProperties(mPhysicalDevice, &memProperties);

    for (uint32_t i { 0 }; i < memProperties.memoryTypeCount; ++i)
    {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
        {
            return i;
        }
    }

    SCARLETT_FLOG("Failed to find suitable memory type.");
    throw std::runtime_error("Failed to find suitable memory type.");
}

VkFormat Device::FindSupportedFormat(const std::vector<VkFormat>& candidates, const VkImageTiling tiling, const VkFormatFeatureFlags features) const
{
    for (const VkFormat format : candidates)
    {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(mPhysicalDevice, format, &props);

        if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
        {
            return format;
        }

        if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
        {
            return format;
        }
    }

    throw std::runtime_error("Failed to find supported format!");
}

void Device::CreateImageWithInfo(const VkImageCreateInfo& imageInfo, const VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory) const
{
    VK_CHECK(vkCreateImage(mDevice, &imageInfo, nullptr, &image), "Failed to create Vulkan Image.");

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(mDevice, image, &memRequirements);

    const VkMemoryAllocateInfo allocInfo
    {
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .allocationSize     = memRequirements.size,
        .memoryTypeIndex    = FindMemoryType(memRequirements.memoryTypeBits, properties),
    };

    VK_CHECK(vkAllocateMemory(mDevice, &allocInfo, nullptr, &imageMemory), "Failed to allocate Vulkan Image Memory.");

    VK_CHECK(vkBindImageMemory(mDevice, image, imageMemory, 0), "Failed to bind Vulkan Image Memory");
}

void Device::CreateBuffer(const VkDeviceSize size, const VkBufferUsageFlags usage, const VkMemoryPropertyFlags properties, VkBuffer &buffer, VkDeviceMemory &bufferMemory) const
{
    const VkBufferCreateInfo bufferInfo
    {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size           = size,
        .usage          = usage,
        .sharingMode    = VK_SHARING_MODE_EXCLUSIVE
    };

    VK_CHECK(vkCreateBuffer(mDevice, &bufferInfo, nullptr, &buffer), "Failed to create Vulkan Buffer");

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(mDevice, buffer, &memRequirements);

    const VkMemoryAllocateInfo allocInfo
    {
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .allocationSize     = memRequirements.size,
        .memoryTypeIndex    = FindMemoryType(memRequirements.memoryTypeBits, properties)
    };

    VK_CHECK(vkAllocateMemory(mDevice, &allocInfo, nullptr, &bufferMemory), "Failed to allocate Vulkan Buffer Memory.");

    vkBindBufferMemory(mDevice, buffer, bufferMemory, 0);
}

void Device::CreateInstance()
{
    VkApplicationInfo appInfo
    {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName    = "Scarlett - Application",
        .applicationVersion  = VK_MAKE_VERSION(1, 0, 0),
        .pEngineName         = "Scarlett Engine",
        .engineVersion       = VK_MAKE_VERSION(1, 0, 0),
        .apiVersion          = VK_API_VERSION_1_3
    };

    vector<const char*> extensions = GetRequiredExtensions();

    if (!CheckExtensionsSupported(extensions))
    {
        throw std::runtime_error("Vulkan Instance does not support required extensions.");
    }

    VkInstanceCreateInfo createInfo
    {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo           = &appInfo,
        .enabledExtensionCount      = static_cast<uint32>(extensions.size()),
        .ppEnabledExtensionNames    = extensions.data()
    };

#ifdef SCARLETT_DEBUG
    mValidationLayers.push_back("VK_LAYER_KHRONOS_validation");

    if (!CheckValidationLayersSupported(mValidationLayers))
    {
        throw std::runtime_error("Vulkan Instance does not support required extensions.");
    }

    createInfo.enabledLayerCount = static_cast<uint32>(mValidationLayers.size());
    createInfo.ppEnabledLayerNames = mValidationLayers.data();
#endif // SCARLETT_DEBUG.

    VK_CHECK(vkCreateInstance(&createInfo, nullptr, &mInstance), "Failed to create Vulkan Instance.");
}

void Device::CreateDebugMessenger()
{
#ifdef SCARLETT_DEBUG
    constexpr VkDebugUtilsMessengerCreateInfoEXT createInfo
    {
        .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        .messageSeverity    = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
        .messageType        = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
        .pfnUserCallback    = DebugCallback,
        .pUserData          = nullptr
    };

    VK_CHECK(CreateDebugUtilsMessengerEXT(mInstance, &createInfo, nullptr, &mDebugMessenger), "Failed to setup vulkan debug messenger.");
#endif // SCARLETT_DEBUG.
}

void Device::CreateSurface()
{
    VK_CHECK(glfwCreateWindowSurface(mInstance, static_cast<GLFWwindow*>(mWindowRef->GetNativeWindow()), nullptr, &mSurface), "Failed to create a Vulkan Surface");
}

void Device::ChoosePhysicalDevice()
{
    uint32 physicalDeviceCount = 0;
    vkEnumeratePhysicalDevices(mInstance, &physicalDeviceCount, nullptr);

    if (physicalDeviceCount == 0)
    {
        throw std::runtime_error("No GPU that support vulkan instance.");
    }

    vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
    vkEnumeratePhysicalDevices(mInstance, &physicalDeviceCount, physicalDevices.data());

    // Find the first physical device that is suitable.
    for (const VkPhysicalDevice& device : physicalDevices)
    {
        if (CheckDeviceSuitable(device))
        {
            mPhysicalDevice = device;

            // Information about the device itself (id, name, type, vendor, etc.).
            VkPhysicalDeviceProperties properties;
            vkGetPhysicalDeviceProperties(device, &properties);

            SCARLETT_DLOG("Vulkan Physical Device Details:\n------------------------------");
            SCARLETT_DLOG("Device Name: {0}", properties.deviceName);
            SCARLETT_DLOG("Vendor ID: {0}", properties.vendorID);
            SCARLETT_DLOG("API Version: {0}", properties.apiVersion);

            VkPhysicalDeviceDescriptorIndexingProperties indexingProps{};
            indexingProps.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_PROPERTIES;

            VkPhysicalDeviceProperties2 props2{};
            props2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
            props2.pNext = &indexingProps;

            vkGetPhysicalDeviceProperties2(device, &props2);

            SCARLETT_DLOG("Descriptor count: {0}", indexingProps.maxDescriptorSetUpdateAfterBindSampledImages);


            break;
        }
    }

    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(mPhysicalDevice, &properties);
}

void Device::CreateLogicalDevice()
{
    mQueueFamilyIndices = GetQueueFamilyIndices(mPhysicalDevice);

    vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    const std::set<int> queueFamilyIndices = { mQueueFamilyIndices.graphicsFamily, mQueueFamilyIndices.presentationFamily };

    for (const uint32 queueFamilyIndex : queueFamilyIndices)
    {
        float priority = 1.0f;
        VkDeviceQueueCreateInfo queueCreateInfo
        {
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .queueFamilyIndex   = queueFamilyIndex,
            .queueCount         = 1,
            .pQueuePriorities   = &priority
        };

        queueCreateInfos.push_back(queueCreateInfo);
    }

    constexpr VkPhysicalDeviceFeatures features { .samplerAnisotropy = VK_TRUE };

    constexpr VkPhysicalDeviceDescriptorIndexingFeatures indexingFeatures
    {
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES,
        .shaderSampledImageArrayNonUniformIndexing      = VK_TRUE,
        .descriptorBindingSampledImageUpdateAfterBind   = VK_TRUE,
        .descriptorBindingPartiallyBound                = VK_TRUE,
        .descriptorBindingVariableDescriptorCount       = VK_TRUE,
        .runtimeDescriptorArray                         = VK_TRUE,
    };

    const VkPhysicalDeviceFeatures2 features2
    {
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
        .pNext = (void*)(&indexingFeatures),
        .features = features
    };

    VkDeviceCreateInfo deviceCreateInfo
    {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext                      = &features2,
        .queueCreateInfoCount       = static_cast<uint32>(queueCreateInfos.size()),
        .pQueueCreateInfos          = queueCreateInfos.data(),
        .enabledExtensionCount      = static_cast<uint32>(mDeviceExtensions.size()),
        .ppEnabledExtensionNames    = mDeviceExtensions.data(),
        .pEnabledFeatures           = nullptr
    };

    VK_CHECK(vkCreateDevice(mPhysicalDevice, &deviceCreateInfo, nullptr, &mDevice), "Failed to create a vulkan logical device.");

    vkGetDeviceQueue(mDevice, mQueueFamilyIndices.graphicsFamily, 0, &mGraphicsQueue);
    vkGetDeviceQueue(mDevice, mQueueFamilyIndices.presentationFamily, 0, &mPresentQueue);
}


void Device::CreateCommandPool()
{
    const VkCommandPoolCreateInfo poolInfo
    {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .pNext              = nullptr,
        .flags              = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT | VK_COMMAND_POOL_CREATE_TRANSIENT_BIT,
        .queueFamilyIndex   = static_cast<uint32>(mQueueFamilyIndices.graphicsFamily)
    };

    VK_CHECK(vkCreateCommandPool(mDevice, &poolInfo, nullptr, &mGraphicsCommandPool), "Failed to create a vulkan [Graphics] command pool.");
}

vector<const char*> Device::GetRequiredExtensions()
{
    uint32 glfwExtensionCount   = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

#ifdef SCARLETT_DEBUG
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif // SCARLETT_DEBUG.

    return extensions;
}

bool Device::CheckExtensionsSupported(const vector<const char*>& extensionsToCheck)
{
    uint32 extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

    for (const char* const& e : extensionsToCheck)
    {
        bool hasExtension = false;
        for (const VkExtensionProperties& check : extensions)
        {
            if (strcmp(e, check.extensionName) == 0)
            {
                hasExtension = true;
                break;
            }
        }

        if (!hasExtension)
        {
            SCARLETT_ELOG("Failed to find extension with name: {0}", e);
            return false;
        }
    }

    return true;
}

bool Device::CheckValidationLayersSupported(const vector<const char*>& validationLayersToCheck)
{
    uint32 layerCount = 0;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    vector<VkLayerProperties> layers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, layers.data());

    for (const char* const& e : validationLayersToCheck)
    {
        bool hasExtension = false;
        for (const VkLayerProperties& check : layers)
        {
            if (strcmp(e, check.layerName) == 0)
            {
                hasExtension = true;
                break;
            }
        }

        if (!hasExtension)
        {
            SCARLETT_ELOG("Failed to find validation layer with name: {0}", e);
            return false;
        }
    }

    return true;
}

QueueFamilyIndices Device::GetQueueFamilyIndices(VkPhysicalDevice physicalDevice) const
{
    QueueFamilyIndices indices;

    uint32 queueFamilyCount;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

    vector<VkQueueFamilyProperties> properties(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, properties.data());

    int i = 0;
    for (const VkQueueFamilyProperties& prop : properties)
    {
        if (prop.queueCount > 0 && prop.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            indices.graphicsFamily = i;
        }

        // Check if the graphics queue is also a presentation queue as well.
        VkBool32 presentationSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, mSurface, &presentationSupport);
        if (prop.queueCount > 0 && presentationSupport)
        {
            indices.presentationFamily = i;
        }


        if (indices.IsValid())
        {
            break;
        }

        ++i;
    }

    return indices;
}

bool Device::CheckDeviceSuitable(const VkPhysicalDevice device) const
{
    VkPhysicalDeviceFeatures features;
    vkGetPhysicalDeviceFeatures(device, &features);

    const QueueFamilyIndices indices = GetQueueFamilyIndices(device);

    const bool indexingSupported    = CheckDeviceIndexingSupport(device);
    const bool extensionsSupported  = CheckDeviceExtensionSupport(device);

    const SwapChainDetails swapChainDetails = GetSwapChainDetails(device);
    const bool swapChainValid = !swapChainDetails.formats.empty() && !swapChainDetails.presentationModes.empty();

    return indices.IsValid() && indexingSupported && extensionsSupported && swapChainValid && features.samplerAnisotropy;
}

bool Device::CheckDeviceExtensionSupport(const VkPhysicalDevice device) const
{
    uint32 propertiesCount = 0;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &propertiesCount, nullptr);

    if (propertiesCount == 0)
    {
        return false;
    }

    vector<VkExtensionProperties> extensions(propertiesCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &propertiesCount, extensions.data());

    for (const char* const& extension : mDeviceExtensions)
    {
        bool hasExtensions = false;
        for (const VkExtensionProperties& e : extensions)
        {
            if (strcmp(extension, e.extensionName) == 0)
            {
                hasExtensions = true;
                break;
            }
        }

        if (!hasExtensions)
        {
            SCARLETT_ELOG("Failed to find extension with name: {0}", extension);
            return false;
        }
    }

    return true;
}

bool Device::CheckDeviceIndexingSupport(const VkPhysicalDevice device)
{
    constexpr VkPhysicalDeviceDescriptorIndexingFeatures indexingFeatures
    {
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES,
    };

    VkPhysicalDeviceFeatures2 deviceFeatures
    {
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
        .pNext = (void*)(&indexingFeatures)
    };

    vkGetPhysicalDeviceFeatures2(device, &deviceFeatures);

    if (!indexingFeatures.runtimeDescriptorArray || !indexingFeatures.shaderSampledImageArrayNonUniformIndexing)
    {
        SCARLETT_FLOG("Device does not support required descriptor indexing features.");
        throw std::runtime_error("Device does not support required descriptor indexing features.");
        return false;
    }

    return true;
}

SwapChainDetails Device::GetSwapChainDetails(const VkPhysicalDevice device) const
{
    SwapChainDetails swapChainDetails;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, mSurface, &swapChainDetails.surfaceCapabilities);

    {
        uint32 surfaceFormatsCount = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, mSurface, &surfaceFormatsCount, nullptr);

        if (surfaceFormatsCount != 0)
        {
            swapChainDetails.formats.resize(surfaceFormatsCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, mSurface, &surfaceFormatsCount, swapChainDetails.formats.data());
        }
    }

    {
        uint32 surfacePresentationCount = 0;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, mSurface, &surfacePresentationCount, nullptr);

        if (surfacePresentationCount != 0)
        {
            swapChainDetails.presentationModes.resize(surfacePresentationCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, mSurface, &surfacePresentationCount, swapChainDetails.presentationModes.data());
        }
    }

    return swapChainDetails;
}

}  // Namespace Scarlett.
