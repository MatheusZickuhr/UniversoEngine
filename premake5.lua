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


include "libs/glfw.lua"
include "libs/glad.lua"
include "libs/glm.lua"
include "libs/imgui.lua"
include "libs/stb_image.lua"
include "libs/OBJ_Loader.lua"
include "UniversoEngine/universoEngine.lua"
include "Example/example.lua"