include "librariesLinks.lua"
include "librariesIncludeDirectories.lua"

project "UniversoEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	architecture "x86_64"
	staticruntime "off"
	location "%{wks.location}"
	targetdir "%{wks.location}/bin/%{cfg.buildcfg}"
	objdir "%{wks.location}/obj/%{cfg.buildcfg}"

	files { "src/**.cpp", "src/**.h" }

	librariesIncludeDirectories()

	librariesLinks()
    