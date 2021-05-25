include "../UniversoEngine/librariesLinks.lua"
include "../UniversoEngine/librariesIncludeDirectories.lua"

project "Example"
	kind "ConsoleApp"
    language "C++"
	cppdialect "C++17"
	architecture "x86_64"
	staticruntime "off"
	location "%{wks.location}"
	targetdir "%{wks.location}/bin/%{cfg.buildcfg}"
	objdir "%{wks.location}/obj/%{cfg.buildcfg}"
	
    files { "src/**.cpp", "src/**.h" }
    
    includedirs { "%{wks.location}/UniversoEngine/src/" }
    librariesIncludeDirectories()
    
    links { "UniversoEngine" }
    librariesLinks()
