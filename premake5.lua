workspace "OpenGL Physics Renderer"
    architecture "x64"
    startproject "VulkanAPI"

    configurations { "Debug", "Release" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder
IncludeDir = {}
IncludeDir["GLFW"] = "../vendor/GLFW/include"
IncludeDir["GLM"] = "../vendor/GLM"
IncludeDir["GLEW"] = "../vendor/GLEW/include"
IncludeDir["JSON"] = "../vendor/JSON"
IncludeDir["STB_COMPLEX"] = "../vendor/STB_COMPLEX"

LibDir = {}
LibDir["Vulkan"] = "%VULKAN_SDK%/Lib"
LibDir["GLFW"] = "../vendor/GLFW/lib"
LibDir["GLEW"] = "../vendor/GLEW/lib"


group "OpenGLPhysics"
    include "OpenGLPhysics/OpenGLPhysics.lua"