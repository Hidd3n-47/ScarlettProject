-- ---------------------------- Scarlett Editor Project.
project "ScarlettEditor"
    location "ScarlettEditor"
    kind "SharedLib"
    language "C++"
    staticruntime "On"
    cppdialect "C++20"

    targetdir(outputPath .. "%{prj.name}")
    objdir(outputIntPath .. "%{prj.name}")

    pchheader "ScarlettEditorPch.h"
    pchsource "%{prj.name}/Src/ScarlettEditorPch.cpp"

    files
    {
        "%{prj.name}/**.h",
        "%{prj.name}/**.cpp",
    }

    includedirs
    {
        "$(SolutionDir)%{prj.name}/%{prj.name}/",
        "$(SolutionDir)%{prj.name}/%{prj.name}/Src",

        "$(SolutionDir)deps/scarlettLogger/",
        "$(SolutionDir)deps/scarlettLogger/deps/",
        "$(SolutionDir)deps/imgui/",
        "$(SolutionDir)deps/include/",

        "$(SolutionDir)ScarlettGameCore",
        "$(SolutionDir)ScarlEntt/ScarlEntt/",
    }

    links
    {
        "ScarlettLogger",
        "ScarlettGameCore",
        "ScarlEntt",
        "ImGui"
    }

    postbuildcommands
    {
        ("{COPYFILE} %[%{cfg.buildtarget.abspath}] %[" .. outputPath .."ScarlettEngine/]"),
        ("{COPYFILE} %[%{cfg.buildtarget.abspath}] %[" .. outputPath .."Scarlett/]")
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Dev"
        runtime "Debug"
        defines { "SCARLETT_EDITOR_DEBUG", "SCARLETT_EDITOR_EXPORT" }
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        defines "SCARLETT_EDITOR_RELEASE"
        optimize "on"
