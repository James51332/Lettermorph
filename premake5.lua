workspace "Lettermorph"
	location "build"
	staticruntime "On"
	cppdialect "C++17"
	architecture "x64"
	startproject "Lettermorph"

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

project "Lettermorph"
	location "build"
	kind "ConsoleApp"
	
	targetdir "build/bin/Lettermorph-%{cfg.system}/%{cfg.shortname}"
	objdir "build/bin-obj/Lettermorph-%{cfg.system}/%{cfg.shortname}"

	files
	{
		"src/**.cpp",
		"src/**.h"	
	}

	sysincludedirs
	{
		"thirdparty/SDL3/include",
  	"thirdparty/SDL_image/include"
	}
	
	libdirs
	{
		"thirdparty/SDL3/lib/%{cfg.system}/%{cfg.shortname}",
  	"thirdparty/SDL_image/lib/%{cfg.system}/%{cfg.shortname}"
	}

	links
	{
		"SDL3",
  	"SDL3_image"
	}

	filter "configurations:Debug"
		symbols "On"
		optimize "Off"

	filter "configurations:Release"
		symbols "Off"
		optimize "On"

	filter "configurations:Dist"
	symbols "Off"
		optimize "Full"

	filter "system:macosx"
		links
		{
			"Cocoa.framework",
			"QuartzCore.framework",
			"OpenGL.framework"
		}
