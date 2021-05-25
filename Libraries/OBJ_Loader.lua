project "OBJ_Loader"
	kind "StaticLib"
	language "C++"
	architecture "x86_64"
	systemversion "latest"
	staticruntime "On"
	targetdir "%{wks.location}/bin/%{cfg.buildcfg}"
	objdir "%{wks.location}/obj/%{cfg.buildcfg}"
	
	includedirs { "OBJ_Loader/include/" }

	files {
		"OBJ_Loader/**.h",
		"OBJ_Loader/**.cpp"
	}
	
	filter "system:linux"
		pic "On"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"