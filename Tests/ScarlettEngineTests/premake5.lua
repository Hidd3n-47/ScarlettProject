-- ---------------------------- ScarlettEngineTests project.
project "ScarlettEngineTests"
    location "ScarlettEngineTests"
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
        "$(SolutionDir)ScarlettEngine/ScarlettEngine/",
    }

    links
    {
       "ScarlettTestProject",
       "ScarlettEngine"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Dev"
        symbols "on"
        runtime "Debug"

    filter "configurations:Release"
        optimize "on"
        runtime "Release"
