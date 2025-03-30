-- ---------------------------- ScarlettMathTests project.
project "ScarlettMathTests"
    location "ScarlettMathTests"
    kind "ConsoleApp"
    language "C++"
    staticruntime "On"
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
        "$(SolutionDir)ScarlettMath/ScarlettMath/",

        "$(SolutionDir)deps/Include/",
    }

    links
    {
       "ScarlettTestProject",
       "ScarlettMath"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Dev"
        symbols "on"
        runtime "Debug"

    filter "configurations:Release"
        optimize "on"
        runtime "Release"
