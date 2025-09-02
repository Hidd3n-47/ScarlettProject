-- ---------------------------- ScarlettUtilsTests project.
project "ScarlettUtilsTests"
    location "ScarlettUtilsTests"
    kind "ConsoleApp"
    language "C++"
    staticruntime "Off"
    cppdialect "C++20"

    targetdir("$(SolutionDir)bin/" .. outputNameTests .. "/%{prj.name}")
    objdir("$(SolutionDir)bin-int/" .. outputNameTests .. "/%{prj.name}")

    files
    {
        "%{prj.name}/**.h",
        "%{prj.name}/**.cpp",
    }

    includedirs
    {
        "$(SolutionDir)Tests/%{prj.name}/%{prj.name}/",
        "$(SolutionDir)Tests/ScarlettTestProject/",

        "$(SolutionDir)ScarlettUtils/",
        "$(SolutionDir)ScarlettUtils/ScarlettUtils/",

        "$(SolutionDir)deps/Include/",
    }

    links
    {
       "ScarlettTestProject",
       "ScarlettUtils"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Dev"
        symbols "on"
        runtime "Debug"

    filter "configurations:Release"
        optimize "on"
        runtime "Release"
