-- ---------------------------- Scarlett Game Project.
project "ScarlettGame"
    location "ScarlettGame"
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
    }

    postbuildcommands
    {
        ("{COPYFILE} %[%{cfg.buildtarget.abspath}] %[" .. outputPath .."ScarlettEngine/]"),
        ("{COPYFILE} %[%{cfg.buildtarget.abspath}] %[" .. outputPath .."ScarlettEditor/]")
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Dev"
        runtime "Debug"
        defines "SCARLETT_GAME_DEBUG"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        defines "SCARLETT_GAME_RELEASE"
        optimize "on"
