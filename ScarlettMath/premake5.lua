-- ---------------------------- ScarlettMath project.
project "ScarlettMath"
    location "ScarlettMath"
    kind "StaticLib"
    language "C++"
    staticruntime "Off"
    cppdialect "C++20"

    targetdir(outputPath .. "%{prj.name}")
    objdir(outputIntPath .. "%{prj.name}")

    files
    {
        "%{prj.name}/**.h",
        "%{prj.name}/**.cpp",
    }

    includedirs
    {
        "$(SolutionDir)%{prj.name}/%{prj.name}/",

        "$(SolutionDir)deps/include/",
    }

    postbuildcommands
    {
        ("{COPYFILE} %[%{cfg.buildtarget.abspath}] %[" .. outputPath .."ScarlettGameCore/]"),
        ("{COPYFILE} %[%{cfg.buildtarget.abspath}] %[" .. outputPath .."ScarlettEditor/]")
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Dev"
        defines "SCARLETT_MATH_DEBUG"
        symbols "On"
        runtime "Debug"

    filter "configurations:Release"
        defines "SCARLETT_MATH_RELEASE"
        optimize "On"
        runtime "Release"
