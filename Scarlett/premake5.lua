-- ---------------------------- Scarlett Project.
project "Scarlett"
    location "Scarlett"
    kind "ConsoleApp"
    language "C++"
    staticruntime "On"
    cppdialect "C++20"

    targetdir(outputPath .. "%{prj.name}")
    objdir(outputIntPath .. "%{prj.name}")

    files
    {
        "%{prj.name}/**.h",
        "%{prj.name}/**.cpp"
    }

    includedirs
    {
        "$(SolutionDir)ScarlettEngine/"
    }

    links
    {
        "ScarlettEngine"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Dev"
        defines "SCARLETT_DEBUG"
        symbols "On"
        runtime "Debug"

    filter "configurations:Release"
        defines "SCARLETT_RELEASE"
        optimize "On"
        runtime "Release"
