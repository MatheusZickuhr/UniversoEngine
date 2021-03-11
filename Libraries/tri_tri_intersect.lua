project "tri_tri_intersect"
	kind "StaticLib"
	language "C"
	architecture "x86_64"

	targetdir "%{wks.location}/bin/%{cfg.buildcfg}"
	objdir "%{wks.location}/obj/%{cfg.buildcfg}"
	
	includedirs { "tri_tri_intersect/include/" }

	files
	{
		"tri_tri_intersect/**.h",
		"tri_tri_intersect/**.c"
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