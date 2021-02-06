project "UniversoEngine"
	kind "StaticLib"
	language "C++"
	architecture "x86_64"
	staticruntime "on"

	targetdir "../bin/%{cfg.buildcfg}"
	objdir "../obj/%{cfg.buildcfg}"
	
    includedirs { 
		"../include/",
		"../libs/glad/include/",
		"../libs/glfw/include/",
		"../libs/glm/",
		"../libs/imgui/",
		"../libs/imgui/examples",
		"../libs/stb_image/include/",
		"../libs/OBJ_Loader/include/"
	}
    
    files { "src/**.cpp", "src/**.h" }

    links { "GLFW", "GLM", "GLAD", "ImGui" }

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