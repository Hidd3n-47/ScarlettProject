-- ---------------------------- ScarlettDevOps.
project "ScarlettDevOps"
    location "ScarlettDevOps"
    kind "WindowedApp"
    entrypoint "mainCRTStartup"
    language "C++"
    
    targetdir("bin/" .. outputName .. "/%{prj.name}")
    objdir("bin-int/" .. outputName .. "/%{prj.name}")

    files
    {
        "%{prj.name}/**.h",
        "%{prj.name}/**.cpp"
    }

    removefiles 
    { 
        "%{prj.name}/deps/**.h",
        "%{prj.name}/deps/**.cpp", 
    }
    
    includedirs
    {
        "$(SolutionDir)ScarlEntt/",
        "$(SolutionDir)ScarlettDevOps/",
        "$(SolutionDir)ScarlettDevOps/src/",
        "$(SolutionDir)/deps/ImGui_SDL/",
        "$(SolutionDir)/deps/Include/"
    }

    libdirs
    {
        "$(SolutionDir)/deps/Lib/"
    }

    links
    {
        "ScarlEntt",
        "ImGui_SDL"
    }

    prebuildcommands
    {
        ("{COPY} %[$(SolutionDir)/deps/Lib/SDL2.dll] %[$(SolutionDir)bin/%{outputName}/ScarlettDevOps/]")
    }

    filter "system:windows"
        cppdialect "C++20"
        systemversion "latest"

    filter "configurations:Debug"
        defines "SDO_DEBUG"
        staticruntime "Off"
        symbols "On"

    filter "configurations:Release"
        defines "SDO_RELEASE"
        staticruntime "Off"
        optimize "On"

