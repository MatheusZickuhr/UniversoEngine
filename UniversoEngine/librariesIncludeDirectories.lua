function librariesIncludeDirectories()
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
		"%{wks.location}/Libraries/PerlinNoise/"
	}
end