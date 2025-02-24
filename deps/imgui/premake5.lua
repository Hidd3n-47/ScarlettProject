project "ImGui"
    kind "StaticLib"
    language "C++"
    staticruntime "on"
    cppdialect "C++17"

    targetdir("$(SolutionDir)/bin/" .. outputName .. "/%{prj.name}")
    objdir("$(SolutionDir)/bin-int/" .. outputName .. "/%{prj.name}")

    files
    {
        "imgui/*.h",
        "imgui/*.cpp",
    }

    includedirs
    {
        "$(SolutionDir)deps/include/",
        "%{VulkanPath}/Include/"
    }

    removefiles
    {
        "%{prj.name}/backends/**.h",
        "%{prj.name}/backends/**.cpp"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Dev"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"
