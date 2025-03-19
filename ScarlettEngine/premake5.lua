-- ---------------------------- Scarlett Engine Project.
project "ScarlettEngine"
    location "ScarlettEngine"
    kind "StaticLib"
    language "C++"
    staticruntime "on"
    cppdialect "C++20"

    targetdir(outputPath .. "%{prj.name}")
    objdir(outputIntPath .. "%{prj.name}")

    pchheader "ScarlettEnginePch.h"
    pchsource "%{prj.name}/Src/ScarlettEnginePch.cpp"

    files
    {
        "%{prj.name}/**.h",
        "%{prj.name}/**.cpp",
    }

    includedirs
    {
        "$(SolutionDir)%{prj.name}/%{prj.name}/",
        "$(SolutionDir)%{prj.name}/%{prj.name}/Src",
        "$(SolutionDir)%{prj.name}/%{prj.name}/deps/include/",

        "$(SolutionDir)ScarlettEditor/",
        "$(SolutionDir)ScarlettEditor/ScarlettEditor/",

        "$(SolutionDir)ScarlettGameCore/",

        "$(SolutionDir)ScarlEntt/ScarlEntt/",

        "$(SolutionDir)deps/include/",
        "$(SolutionDir)deps/imgui/",
        "$(SolutionDir)deps/scarlettLogger/",
        "$(SolutionDir)deps/scarlettLogger/deps/",

        "%{VulkanPath}/Include/"
    }

    libdirs
    {
        "$(SolutionDir)deps/lib/",
        "%{VulkanPath}/Lib/"
    }

    links
    {
        "ScarlettLogger",
        "ScarlettEditor",
        "ScarlettGameCore",
        "ScarlEntt",
        "ImGui",
        "glfw3_mt.lib",
        "vulkan-1.lib"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Dev"
        runtime "Debug"
        defines { "SCARLETT_DEBUG", "SCARLETT_EDITOR_ENABLED" }
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        defines "SCARLETT_RELEASE"
        optimize "on"
