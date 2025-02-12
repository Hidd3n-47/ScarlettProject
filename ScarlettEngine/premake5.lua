-- ---------------------------- Scarlett Engine Project.
project "ScarlettEngine"
    location "ScarlettEngine"
    kind "StaticLib"
    language "C++"
	staticruntime "on"
	cppdialect "C++20"
    
    targetdir("$(SolutionDir)/bin/" .. outputName .. "/%{prj.name}")
    objdir("$(SolutionDir)/bin-int/" .. outputName .. "/%{prj.name}")

    pchheader "ScarlettEnginePch.h"
    pchsource "%{prj.name}/Src/ScarlettEnginePch.cpp"
	
	VulkanPath = os.getenv('VULKAN_SDK')
    
    files
    {
        "%{prj.name}/**.h",
        "%{prj.name}/**.cpp",
    }
    
   files
    {
        "$(SolutionDir)deps/include/vkbootstrap/**.h",
        "$(SolutionDir)deps/include/vkbootstrap/**.cpp",
    }
	
    includedirs
    {
		"$(SolutionDir)%{prj.name}/%{prj.name}/",
		"$(SolutionDir)%{prj.name}/%{prj.name}/Src",
		"$(SolutionDir)%{prj.name}/%{prj.name}/deps/include/",
		
		"$(SolutionDir)deps/include/",
		"$(SolutionDir)deps/imgui/",
        "%{VulkanPath}/Include/"
    }

    libdirs 
    { 
		"$(SolutionDir)deps/lib/",
        "%{VulkanPath}/Lib/"
    }

    links 
    {
		"ImGui",
		"glfw3_mt.lib",
        "vulkan-1.lib"
    }
	
    filter "system:windows"
        systemversion "latest"

    filter "configurations:Dev"
        runtime "Debug"
        defines "SCARLETT_DEBUG"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        defines "SCARLETT_RELEASE"
        optimize "on"
