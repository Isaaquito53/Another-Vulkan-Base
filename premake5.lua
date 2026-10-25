workspace "AnotherVulkanBase"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	flags
	{
		"MultiProcessorCompile"
	}
	
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "AnotherVulkanBase"
	location "."
	kind "ConsoleApp"
	language "C++"
	
	targetdir ("./")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
	files
	{
		"VulkanCore/src/**.h",
		"VulkanCore/src/**.cpp",
		"NoVulkan/src/**.h",
		"NoVulkan/src/**.cpp"
	}
	
	includedirs
	{
		"VulkanCore/src",
		"VulkanCore/src/vendor",
		"VulkanCore/include",
		"NoVulkan/src",
		"C:/VulkanSDK/1.4.304.1/Include"
	}
	
	libdirs
	{
		"VulkanCore/lib"
	}

	links
	{
		"vulkan-1.lib",
		"glfw3.lib"
	}
	
	filter "system:windows"
		cppdialect "C++17"
		staticruntime "Off"
		systemversion "latest"

	filter "configurations:Debug"
		defines "_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "_DIST"
		optimize "On"