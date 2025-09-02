-- ---------------------------- ScarlENTT project.
project "ScarlEntt"
    location "ScarlEntt"
    kind "StaticLib"
    language "C++"
    staticruntime "Off"
    cppdialect "C++20"

    targetdir(outputPath .. "%{prj.name}")
    objdir(outputIntPath .. "%{prj.name}")

    pchheader "ScarlEnttpch.h"
    pchsource "%{prj.name}/ScarlEntt/ScarlEnttpch.cpp"

    dependson { "ScarlettBuild" }

    files
    {
        "%{prj.name}/**.h",
        "%{prj.name}/**.cpp",
    }

    includedirs
    {
        "$(SolutionDir)Scarlett/",
        "$(SolutionDir)ScarlettUtils/",
        "$(SolutionDir)ScarlEntt/ScarlEntt/",
        "$(SolutionDir)ScarlettMath/ScarlettMath/",

        "$(SolutionDir)deps/include/",
    }

    links
    {
        "ScarlettMath",
        "ScarlettUtils",
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Dev"
        defines { "DEV_CONFIGURATION", "SCARLENTT_DEBUG" }
        symbols "on"
        runtime "Debug"

    filter "configurations:Release"
        defines "SCARLENTT_RELEASE"
        optimize "on"
        runtime "Release"
