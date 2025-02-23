workspace "GameEngine"
    architecture "x64"
    startproject "Sandbox"

    configurations 
    {
        "Debug",
        "Release",
        "Dist"
    }

-- ad es.: DEBUG-WINDOWS-X64
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "GameEngine/vendor/GLFW/include"
IncludeDir["Glad"] = "GameEngine/vendor/Glad/include"
IncludeDir["ImGui"] = "GameEngine/vendor/imgui"

-- Con questa istruzione, i file premake di queste librerie vengono inseriti in questo file
include "GameEngine/vendor/GLFW"
include "GameEngine/vendor/Glad"
include "GameEngine/vendor/imgui"

project "GameEngine"
    location "GameEngine"
    kind "SharedLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "hzpch.h"
    pchsource "GameEngine/src/hzpch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",    
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}"
    }

    links
    {
        "GLFW",
        "Glad",
        "ImGui",
        "opengl32.lib",
        "dwmapi.lib"
    }

    -- I filtri sono utili per configurazioni che si applicano a una specifica piattaforma
    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines 
        {
            "HZ_PLATFORM_WINDOWS",
            "HZ_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }

        postbuildcommands
        {
            -- Va nella cartella in cui si trova GameEngine.dll e lo copia nella cartella Sandbox
            ("{COPYFILE} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
        }
    
    filter "configurations:Debug"
        defines "HZ_DEBUG"
        buildoptions "/MDd"
        symbols "On"
    
    filter "configurations:Release"
        defines "HZ_RELEASE"
        buildoptions "/MD"
        optimize "On"

    filter "configurations:Dist"
        defines "HZ_DIST"
        buildoptions "/MD"
        optimize "On"

    filter "action:vs2022"
        buildoptions { "/utf-8" }



project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "GameEngine/vendor/spdlog/include",
        "GameEngine/src"
    }

    links
    {
        "GameEngine"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines 
        {
            "HZ_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "HZ_DEBUG"
        buildoptions "/MDd"
        symbols "On"
    
    filter "configurations:Release"
        defines "HZ_RELEASE"
        buildoptions "/MD"
        optimize "On"

    filter "configurations:Dist"
        defines "HZ_DIST"
        buildoptions "/MD"
        optimize "On"

    filter "action:vs2022"
        buildoptions { "/utf-8" }
