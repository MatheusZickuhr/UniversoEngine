project "hash-library"
	kind "StaticLib"
	language "C"
	architecture "x86_64"
	systemversion "latest"
	staticruntime "off"
	targetdir "%{wks.location}/bin/%{cfg.buildcfg}"
	objdir "%{wks.location}/obj/%{cfg.buildcfg}"

	includedirs { "hash-library/" }

	files {
		"hash-library/*.h", "hash-library/*.cpp"
	}

	filter "system:linux"
		pic "On"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"