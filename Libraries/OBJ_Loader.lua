project "OBJ_Loader"
	kind "StaticLib"
	language "C++"
	architecture "x86_64"

	targetdir "%{wks.location}/bin/%{cfg.buildcfg}"
	objdir "%{wks.location}/obj/%{cfg.buildcfg}"
	
	includedirs { "OBJ_Loader/include/" }

	files
	{
		"OBJ_Loader/include/OBJ_Loader/*.h"
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