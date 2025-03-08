-- ---------------------------- ScarlENTT project.
project "ScarlEntt"
    location "ScarlEntt"
    kind "StaticLib"
    language "C++"
    staticruntime "On"
    cppdialect "C++20"

    targetdir(outputPath .. "%{prj.name}")
    objdir(outputIntPath .. "%{prj.name}")

    pchheader "ScarlEnttpch.h"
    pchsource "%{prj.name}/ScarlEntt/ScarlEnttpch.cpp"

    files
    {
        "%{prj.name}/**.h",
        "%{prj.name}/**.cpp",
    }

    includedirs
    {
        "$(SolutionDir)Scarlett/",
        "$(SolutionDir)ScarlEntt/ScarlEntt/",
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Dev"
        defines "SCARLENTT_DEBUG"
        symbols "on"
        runtime "Debug"

    filter "configurations:Release"
        defines "SCARLENTT_RELEASE"
        optimize "on"
        runtime "Release"
