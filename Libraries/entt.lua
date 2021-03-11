project "entt"
	kind "StaticLib"
	language "C++"
    cppdialect "C++17"
	architecture "x86_64"

	targetdir "%{wks.location}/bin/%{cfg.buildcfg}"
	objdir "%{wks.location}/obj/%{cfg.buildcfg}"
	
	includedirs { "entt/include/" }

	files
	{
		"entt/**.hpp", "entt/**.cpp",
	}
	
	filter "system:linux"
		pic "On"

		systemversion "latest"
		staticruntime "On"


	filter "system:windows"
		systemversion "latest"
		staticruntime "On"


	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"