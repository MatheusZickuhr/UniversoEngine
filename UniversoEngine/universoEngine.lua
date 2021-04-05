project "UniversoEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	architecture "x86_64"
	staticruntime "on"

	targetdir "%{wks.location}/bin/%{cfg.buildcfg}"
	objdir "%{wks.location}/obj/%{cfg.buildcfg}"
	
    includedirs { 
		"%{wks.location}/include/",
		"%{wks.location}/Libraries/glad/include/",
		"%{wks.location}/Libraries/glfw/include/",
		"%{wks.location}/Libraries/glm/",
		"%{wks.location}/Libraries/imgui/",
		"%{wks.location}/Libraries/imgui/examples",
		"%{wks.location}/Libraries/stb_image/include/",
		"%{wks.location}/Libraries/OBJ_Loader/include/",
		"%{wks.location}/Libraries/entt/include/",
		"%{wks.location}/Libraries/reactphysics3d/include/"
	}
    
    files { "src/**.cpp", "src/**.h" }

    links { "GLFW", "GLM", "GLAD", "ImGui", "OBJ_Loader", "stb_image", "entt", "reactphysics3d" }

    filter "system:linux"
        links { "dl", "pthread" }
        defines { "_X11" }

    filter "system:windows"
        defines { "_WINDOWS" }
