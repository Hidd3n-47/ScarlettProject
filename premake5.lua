workspace "Scarlett"
    architecture "x64"

    configurations
    {
        "Dev",
        "Release"
    }

outputName          = "%{cfg.buildcfg}"
outputPath          = "$(SolutionDir)bin/" .. outputName .. "/"
outputIntPath       = "$(SolutionDir)bin-int/" .. outputName .. "/"
outputNameTests     = "%{cfg.buildcfg}/Tests"

VulkanPath = os.getenv('VULKAN_SDK')

include "ScarlettEngine/"
include "ScarlettEditor/"
include "ScarlettGameCore/"
include "Scarlett/"
include "ScarlEntt/"
include "ScarlettMath/"

-- Dependencies directories.
include "deps/ImGui/"
include "deps/scarlettLogger/"

-- Test Directories.
include "Tests/ScarlettTestProject/"
include "Tests/ScarlettEngineTests/"
include "Tests/ScarlEnttTests/"
