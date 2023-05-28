workspace "LearnVulkan"
    location(_ACTION)
    configurations { "Debug", "Release"}
    architecture "x86_64"
    startproject "LearnVulkan"

    flags
	{
		"MultiProcessorCompile"
	}

    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "LearnVulkan"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir("%{wks.location}/obj/" .. outputdir .. "/%{prj.name}")

    pchheader "stdafx.h"
    pchsource "../src/stdafx.cpp"

    files { "../src/**.h", "../src/**.cpp" }

    includedirs
    {
        "../src"
    }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"