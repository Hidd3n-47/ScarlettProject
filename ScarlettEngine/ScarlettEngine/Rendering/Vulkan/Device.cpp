#include "ScarlettEnginePch.h"
#include "Device.h"

#include <glfw/glfw3.h>

#include "VulkanUtils.h"
#include "Core/Window/Window.h"

namespace Scarlett
{

void Device::Init(Window* windowRef)
{
    mWindowRef = windowRef;

    InitVulkan();
}

void Device::Destroy()
{
    vkDestroySurfaceKHR(mInstance, mSurface, nullptr);
    vkDestroyDevice(mDevice, nullptr);

    vkb::destroy_debug_utils_messenger(mInstance, mDebugMessenger);
    vkDestroyInstance(mInstance, nullptr);

    mInstance           = nullptr;
    mDebugMessenger     = nullptr;
    mSurface            = nullptr;
    mPhysicalDevice     = nullptr;
    mDevice             = nullptr;
}


void Device::InitVulkan()
{
    vkb::InstanceBuilder builder;

    bool useValidationLayers = false;

#ifdef SCARLETT_DEBUG
    useValidationLayers = true;
#endif // SCARLETT_DEBUG.

    vkb::Result instanceReturned = builder.set_app_name("Vulkan Engine")
        .request_validation_layers(useValidationLayers)
        .use_default_debug_messenger()
        .require_api_version(1, 3, 0)
        .build();

    vkb::Instance instance = instanceReturned.value();

    mInstance       = instance.instance;
    mDebugMessenger = instance.debug_messenger;

    VK_CHECK(glfwCreateWindowSurface(mInstance, static_cast<GLFWwindow*>(mWindowRef->GetNativeWindow()), nullptr, &mSurface), "Failed to create a Vulkan Surface");

    VkPhysicalDeviceVulkan13Features features
    {
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
        .synchronization2 = true,
        .dynamicRendering = true
    };

    VkPhysicalDeviceVulkan12Features features12
    {
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES,
        .descriptorIndexing = true,
        .bufferDeviceAddress = true
    };

    vkb::PhysicalDeviceSelector selector{ instance };
    vkb::PhysicalDevice physicalDevice = selector
        .set_minimum_version(1, 3)
        .set_required_features_13(features)
        .set_required_features_12(features12)
        .set_surface(mSurface)
        .select()
        .value();

    vkb::DeviceBuilder deviceBuilder{ physicalDevice };

    vkb::Device device = deviceBuilder.build().value();

    mDevice = device.device;
    mPhysicalDevice = physicalDevice.physical_device;

    mGraphicsQueueFamily = device.get_queue_index(vkb::QueueType::graphics).value();
    mGraphicsQueue = device.get_queue(vkb::QueueType::graphics).value();
}

} // Namespace Scarlett.