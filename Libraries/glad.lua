project "GLAD"
	kind "StaticLib"
	language "C"
	architecture "x86_64"
	systemversion "latest"
	staticruntime "On"
	targetdir "%{wks.location}/bin/%{cfg.buildcfg}"
	objdir "%{wks.location}/obj/%{cfg.buildcfg}"

	includedirs { "glad/include/" }

	files { "glad/src/glad.c" }

	filter "system:linux"
		pic "On"
		defines {"_GLAD_X11"}

	filter "system:windows"
		defines { 
			"_GLAD_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"