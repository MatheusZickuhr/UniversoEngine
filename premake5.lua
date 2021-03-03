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


include "Libraries/glfw.lua"
include "Libraries/glad.lua"
include "Libraries/glm.lua"
include "Libraries/imgui.lua"
include "Libraries/stb_image.lua"
include "Libraries/OBJ_Loader.lua"
include "Libraries/tri_tri_intersect.lua"
include "UniversoEngine/universoEngine.lua"
include "Example/example.lua"