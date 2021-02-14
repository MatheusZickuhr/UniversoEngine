project "UniversoEngine"
	kind "StaticLib"
	language "C++"
	architecture "x86_64"
	staticruntime "on"

	targetdir "../bin/%{cfg.buildcfg}"
	objdir "../obj/%{cfg.buildcfg}"
	
    includedirs { 
		"../include/",
		"../Libraries/glad/include/",
		"../Libraries/glfw/include/",
		"../Libraries/glm/",
		"../Libraries/imgui/",
		"../Libraries/imgui/examples",
		"../Libraries/stb_image/include/",
		"../Libraries/OBJ_Loader/include/"
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