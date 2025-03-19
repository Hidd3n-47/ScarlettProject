-- ---------------------------- Scarlett Game Core Project.
project "ScarlettGameCore"
    location "ScarlettGameCore"
    kind "SharedLib"
    language "C++"
    staticruntime "On"
    cppdialect "C++20"

    targetdir(outputPath .. "%{prj.name}")
    objdir(outputIntPath .. "%{prj.name}")

    --pchheader "ScarlettGamePch.h"
    --pchsource "%{prj.name}/Src/ScarlettGamePch.cpp"

    files
    {
        "%{prj.name}/**.h",
        "%{prj.name}/**.cpp",
    }

    includedirs
    {
        "$(SolutionDir)%{prj.name}/",

        "$(SolutionDir)deps/include/",

        "$(SolutionDir)ScarlEntt/ScarlEntt/",
        "$(SolutionDir)deps/scarlettLogger/",
        "$(SolutionDir)deps/scarlettLogger/deps/",
    }

    links
    {
        "ScarlEntt",
        "ScarlettLogger",
    }

    postbuildcommands
    {
        ("{COPYFILE} %[%{cfg.buildtarget.abspath}] %[" .. outputPath .."ScarlettEngine/]"),
        ("{COPYFILE} %[%{cfg.buildtarget.abspath}] %[" .. outputPath .."ScarlettEditor/]"),
        ("{COPYFILE} %[%{cfg.buildtarget.abspath}] %[" .. outputPath .."Scarlett/]")
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Dev"
        runtime "Debug"
        defines "SCARLETT_GAME_CORE_DEBUG"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        defines "SCARLETT_GAME_CORE_RELEASE"
        optimize "on"
