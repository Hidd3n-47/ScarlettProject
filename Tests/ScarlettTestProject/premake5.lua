-- ---------------------------- ScarlettTestProject project.
project "ScarlettTestProject"
    location "ScarlettTestProject"
    kind "StaticLib"
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
		"$(SolutionDir)Tests/ScarlettTestProject/ScarlettTestProject/",
		"$(SolutionDir)deps/scarlettLogger/deps/",
		"$(SolutionDir)deps/scarlettLogger/"
	}
	
	links
	{
		"ScarlettLogger"
	}

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Dev"
        symbols "on"
        runtime "Debug"

    filter "configurations:Release"
        optimize "on"
        runtime "Release"