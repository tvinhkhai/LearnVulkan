function AddVulkanSDK()
    VULKAN_SDK = os.getenv("VULKAN_SDK")
    includedirs "%{VULKAN_SDK}/Include"
    libdirs "%{VULKAN_SDK}/Lib"
    links "vulkan-1"
end

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

    files 
    {
        "../src/**.h"
        , "../src/**.cpp" 
        , "../src/**.inl" 
        , "../libs/glm/glm/**.hpp"
        , "../libs/glm/glm/**.inl"
    }

    defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

    includedirs
    {
        "../src"
        , "../libs/glfw/include"
        , "../libs/glm"
    }
    
    libdirs
    {
        "../libs/glfw/lib-vc2022"
    }

    links
    {
        "glfw3"
    }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
    filter{}

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"
    filter{}

    AddVulkanSDK()