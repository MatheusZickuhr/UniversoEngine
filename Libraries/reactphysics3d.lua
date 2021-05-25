project "reactphysics3d"
	kind "StaticLib"
	language "C++"
    cppdialect "C++17"
	architecture "x86_64"
	staticruntime "off"
	systemversion "latest"
	targetdir "%{wks.location}/bin/%{cfg.buildcfg}"
	objdir "%{wks.location}/obj/%{cfg.buildcfg}"
	
	includedirs { "reactphysics3d/include/" }

	files {
		"reactphysics3d/src/**.hpp", "reactphysics3d/src/**.h", "reactphysics3d/src/**.cpp"
	}
	
	filter "system:linux"
		pic "On"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"