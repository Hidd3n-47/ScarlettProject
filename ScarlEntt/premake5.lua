-- ---------------------------- ScarlENTT project.
project "ScarlEntt"
    location "ScarlEntt"
    kind "StaticLib"
    language "C++"
	staticruntime "on"
	cppdialect "C++20"
    
    targetdir("$(SolutionDir)/bin/" .. outputName .. "/%{prj.name}")
    objdir("$(SolutionDir)/bin-int/" .. outputName .. "/%{prj.name}")
    
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

    libdirs 
    { 
    }

    links 
    {
    }

    defines
    {
        "SCARLENTT_EXPORT"
    }

    postbuildcommands
    {
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