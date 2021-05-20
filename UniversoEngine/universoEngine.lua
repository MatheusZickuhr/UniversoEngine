project "UniversoEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	architecture "x86_64"
	staticruntime "off"
	location "%{wks.location}"
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
		"%{wks.location}/Libraries/reactphysics3d/include/",
		"%{wks.location}/Libraries/hash-library/",
		"%{VULKAN_SDK_DIR}/Include/"
	}
    
    files { "src/**.cpp", "src/**.h" }

    libdirs { "%{wks.location}/Libraries/VulkanDebugSdk/Lib/", "%{VULKAN_SDK_DIR}/Lib" }

    links { 
		"GLFW",
		"GLM",
		"GLAD", 
		"ImGui",
		"OBJ_Loader",
		"stb_image",
		"entt",
		"reactphysics3d",
		"hash-library",
		"vulkan-1", 
    	"VkLayer_utils",
    	"shaderc_sharedd",
    	"spirv-cross-cored",
    	"spirv-cross-glsld",
    	"spirv-cross-hlsld",
    	"SPIRV-Toolsd"
	}

    filter "system:linux"
        links { "dl", "pthread" }
        defines { "_X11" }

    filter "system:windows"
        defines { "_WINDOWS" }
