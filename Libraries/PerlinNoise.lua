project "PerlinNoise"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	architecture "x86_64"
	systemversion "latest"
	staticruntime "On"
	targetdir "%{wks.location}/bin/%{cfg.buildcfg}"
	objdir "%{wks.location}/obj/%{cfg.buildcfg}"
	includedirs { "PerlinNoise/" }

	files {
		"PerlinNoise/*.hpp", "PerlinNoise/*.cpp" 
	}

	filter "system:linux"
		pic "On"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"