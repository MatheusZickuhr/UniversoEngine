workspace "UniversoEngine"
	configurations { "Debug", "Release" }
	startproject "Example"

	flags { "MultiProcessorCompile" }

	filter "configurations:Debug"
		defines { "DEBUG", "DEBUG_SHADER" }
		symbols "On"

	filter "configurations:Release"
		defines { "RELEASE" }
		optimize "Speed"
		flags { "LinkTimeOptimization" }

	filter "system:windows"
		defines { "_WINDOWS" }

	filter "system:linux"
		defines { "_X11" }

VULKAN_SDK_DIR = os.getenv("VULKAN_SDK")

include "UniversoEngine/universoEngine.lua"
include "Example/example.lua"

group "Libraries"
	include "Libraries/glfw.lua"
	include "Libraries/glad.lua"
	include "Libraries/glm.lua"
	include "Libraries/imgui.lua"
	include "Libraries/stb_image.lua"
	include "Libraries/OBJ_Loader.lua"
	include "Libraries/entt.lua"
	include "Libraries/reactphysics3d.lua"
	include "Libraries/hash-library.lua"
