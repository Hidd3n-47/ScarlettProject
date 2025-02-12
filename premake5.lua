workspace "Scarlett"
    architecture "x64"
    
    configurations
    {
        "Dev",
        "Release"
    }
    
outputName = "%{cfg.buildcfg}"
VulkanPath = os.getenv('VULKAN_SDK')
    
include "ScarlettEngine/"
include "Scarlett/"
include "deps/ImGui/"
    