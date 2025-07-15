-- ---------------------------- Scarlett Build Project.
project "ScarlettBuild"
    location "ScarlettBuild"
    kind "ConsoleApp"
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
        "$(SolutionDir)%{prj.name}/",
        "$(SolutionDir)deps/ScarlettLogger/",
        "$(SolutionDir)deps/ScarlettLogger/deps/",
    }

    links
    {
        "ScarlettLogger",
    }

    postbuildcommands {  "call \"%{wks.location}Build\\Build.bat\"" }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Dev"
        runtime "Debug"
        defines { "DEV_CONFIGURATION", "SCARLETT_BUILD_DEBUG" }
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        defines { "SCARLETT_BUILD_RELEASE" }
        optimize "on"
