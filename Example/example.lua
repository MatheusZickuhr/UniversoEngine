project "Example"
	kind "ConsoleApp"
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
        "%{VULKAN_SDK_DIR}/include/",
		"%{wks.location}/UniversoEngine/src/"
	}
    
    files { "src/**.cpp", "src/**.h" }

    libdirs { "%{wks.location}/Libraries/VulkanDebugSdk/Lib/", "%{VULKAN_SDK_DIR}/lib" }

    links {
        "UniversoEngine",
        "GLFW",
        "GLM",
        "GLAD",
        "ImGui",
        "OBJ_Loader",
        "stb_image",
        "entt",
        "reactphysics3d",
        "hash-library",
        "VkLayer_utils"
    }

    -- if is debug and windows
    filter { "configurations:Debug", "system:windows" }
        links {
            "shaderc_combinedd",
            "spirv-cross-cored",
            "spirv-cross-glsld",
            "spirv-cross-hlsld",
            "SPIRV-Toolsd"
        }

    -- if is debug and linux
    filter { "configurations:Debug", "system:linux" }
        links {
            "shaderc_combined",
            "spirv-cross-core",
            "spirv-cross-glsl",
            "spirv-cross-hlsl",
            "SPIRV-Tools"
        }

    filter "configurations:Release"
        links {
            "shaderc_combined",
            "spirv-cross-core",
            "spirv-cross-glsl",
            "spirv-cross-hlsl",
            "SPIRV-Tools"
        }

    filter "system:windows"
        links { "vulkan-1" }
        defines { "_WINDOWS" }

    filter "system:linux"
        links { "vulkan", "dl", "pthread" }
        defines { "_X11" }