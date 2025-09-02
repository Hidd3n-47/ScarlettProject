-- ---------------------------- Scarlett Utils Project.
project "ScarlettUtils"
    location "ScarlettUtils"
    kind "StaticLib"
    language "C++"
    staticruntime "Off"
    cppdialect "C++20"

    targetdir(outputPath .. "%{prj.name}")
    objdir(outputIntPath .. "%{prj.name}")

    pchheader "ScarlettUtilsPch.h"
    pchsource "%{prj.name}/Src/ScarlettUtilsPch.cpp"

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

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Dev"
        runtime "Debug"
        defines "SCARLETT_UTILS_DEBUG"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        defines "SCARLETT_UTILS_RELEASE"
        optimize "on"
