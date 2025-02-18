workspace "Scarlett"
    architecture "x64"
    
    configurations
    {
        "Dev",
        "Release"
    }
    
outputName = "%{cfg.buildcfg}"
outputNameTests = "%{cfg.buildcfg}/Tests"
VulkanPath = os.getenv('VULKAN_SDK')
    
include "ScarlettEngine/"
include "Scarlett/"
include "ScarlEntt/"
    
-- Dependencies directories.
include "deps/ImGui/"
include "deps/scarlettLogger/"

-- Test Directories.
include "Tests/ScarlettTestProject/"
include "Tests/ScarlEnttTests/"