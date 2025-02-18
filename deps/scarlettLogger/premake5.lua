-- ---------------------------- ScarlettLogger project.
project "ScarlettLogger"
    location "ScarlettLogger"
    kind "StaticLib"
    language "C++"
	staticruntime "on"
	cppdialect "C++20"
    
    targetdir("$(SolutionDir)/bin/" .. outputName .. "/%{prj.name}")
    objdir("bin-int/" .. outputName .. "/%{prj.name}")

    files
    {
        "%{prj.name}/**.h",
        "%{prj.name}/**.cpp",
    }
	
	includedirs
	{
		"$(SolutionDir)/deps/scarlettLogger/deps/"
	}

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Dev"
        symbols "on"
        runtime "Debug"

    filter "configurations:Release"
        optimize "on"
        runtime "Release"