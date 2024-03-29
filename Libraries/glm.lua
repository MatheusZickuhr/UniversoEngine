project "GLM"
	kind "StaticLib"
	language "C"
	architecture "x86_64"
	systemversion "latest"
	staticruntime "On"
	targetdir "%{wks.location}/bin/%{cfg.buildcfg}"
	objdir "%{wks.location}/obj/%{cfg.buildcfg}"

	includedirs { "glm/" }

	files {
		"glm/glm/**"
	}

	filter "system:linux"
		pic "On"
		defines {
			"_GLM_X11"
		}

	filter "system:windows"
		defines { 
			"_GLM_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"