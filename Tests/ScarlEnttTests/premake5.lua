-- ---------------------------- ScarlEnttTests project.
project "ScarlEnttTests"
    location "ScarlEnttTests"
    kind "ConsoleApp"
    language "C++"
	staticruntime "on"
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
		"$(SolutionDir)Tests/ScarlettTestProject/",
		"$(SolutionDir)ScarlEntt/ScarlEntt/",
		"$(SolutionDir)Tests/ScarlEnttTests/ScarlEnttTests/"
	}
	
	links
	{
		"ScarlettTestProject",
		"ScarlEntt"
	}

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Dev"
        symbols "on"
        runtime "Debug"

    filter "configurations:Release"
        optimize "on"
        runtime "Release"