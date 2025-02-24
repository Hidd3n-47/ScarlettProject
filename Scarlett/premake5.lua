-- ---------------------------- Scarlett Project.
project "Scarlett"
    location "Scarlett"
    kind "ConsoleApp"
    language "C++"
    staticruntime "on"
    cppdialect "C++20"

    targetdir("$(SolutionDir)/bin/" .. outputName .. "/%{prj.name}")
    objdir("$(SolutionDir)/bin-int/" .. outputName .. "/%{prj.name}")

    files
    {
        "%{prj.name}/**.h",
        "%{prj.name}/**.cpp"
    }

    removefiles
    {
        "%{prj.name}/deps/**.h",
        "%{prj.name}/deps/**.cpp"
    }

    includedirs
    {
        "$(SolutionDir)ScarlettEngine/"
    }

    links
    {
        "ScarlettEngine"
    }

     postbuildcommands
    {
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
