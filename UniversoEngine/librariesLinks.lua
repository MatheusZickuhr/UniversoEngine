function librariesLinks()
	
	libdirs { "%{wks.location}/Libraries/VulkanDebugSdk/Lib/", "%{VULKAN_SDK_DIR}/lib" }

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

	filter "system:linux"
		links { "vulkan", "dl", "pthread" }

end