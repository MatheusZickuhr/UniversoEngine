project "stb_image"
	kind "StaticLib"
	language "C"
	architecture "x86_64"

	targetdir "../bin/%{cfg.buildcfg}"
	objdir "../obj/%{cfg.buildcfg}"
	
	includedirs { "stb_image/include/" }

	files
	{
		"stb_image/include/stb_image/*.h", "stb_image/*.cpp",
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