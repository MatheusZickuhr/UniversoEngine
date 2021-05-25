project "ImGui"
	kind "StaticLib"
	language "C"
	architecture "x86_64"
	systemversion "latest"
	staticruntime "On"
	targetdir "%{wks.location}/bin/%{cfg.buildcfg}"
	objdir "%{wks.location}/obj/%{cfg.buildcfg}"
	
	includedirs { "imgui/", "imgui/examples/", "glad/include", "glfw/include/" }

	files {
		"imgui/*.cpp",
		"imgui/examples/imgui_impl_glfw.cpp",
		"imgui/examples/imgui_impl_opengl3.cpp"
	}

	defines { "IMGUI_IMPL_OPENGL_LOADER_GLAD" }
    
	filter "system:linux"
		pic "On"

		defines {"_IMGUI_X11" }

	filter "system:windows"
		defines { 
			"_IMGUI_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"