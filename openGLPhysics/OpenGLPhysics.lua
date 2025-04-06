project "OpenGLAPI"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++latest"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    -- Include directories
    includedirs 
    {
        "%{IncludeDir.GLEW}",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.JSON}",
        "%{IncludeDir.GLM}",
        "%{IncludeDir.STB_COMPLEX}",
        "global",            
        "include",           
        "shaders",
        "opengl-physics",
        "json",
        "src"
    }

    -- Files
    files 
    {
        "src/**.cpp",           
        "opengl-physics/tools/src/**.cpp", 
        "opengl-physics/glInit/src/**.cpp",
        "opengl-physics/glUtil/src/**.cpp", 
        "opengl-physics/physics/src/**.cpp", 
        "include/**.h",           
        "opengl-physics/tools/include/**.h", 
        "opengl-physics/glInit/include/**.h",
        "opengl-physics/glUtil/include/**.h", 
        "opengl-physics/physics/include/**.h",

        "opengl-physics/tools/include/**.inl", 
        "opengl-physics/glInit/include/**.inl",
        "opengl-physics/glUtil/include/**.inl",
        "opengl-physics/physics/include/**.inl",
        "global/**.h",
        "global/**.cpp",
        "json/**.json",
        "shaders/**.vert",
        "shaders/**.frag"
    }

    -- Library directories
    libdirs 
    { 
        "%{LibDir.GLEW}",
        "%{LibDir.GLFW}"
    }

    -- Links
    links 
    { 
        "glew32s.lib",
        "opengl32.lib",
        "glfw3_mt.lib",
        "libcmt.lib",
        "kernel32.lib"
    }

    pchheader "headers.h"
    pchsource "global/headers.cpp"

    defines {"GLEW_STATIC"}

    flags { "Verbose" }


    -- Toolset and compiler settings
    filter "toolset:msc"
        toolset "msc-v143" --
        buildoptions { "/std:c++23" } 
        
    filter "toolset:gcc or toolset:clang"
        buildoptions { "-std=c++23" }

    -- Configuration settings
    filter "configurations:Debug"
        defines "DEBUG"
        symbols "On"
        optimize "Off"
        runtime "Release"  

    filter "configurations:Release"
        symbols "Off"
        optimize "On"
        defines "NDEBUG"
        runtime "Release"  

    -- Windows system settings
    filter "system:windows"
        systemversion "latest"
        defines "PLATFORM_WINDOWS"
    
    -- Visual Studio specific settings
    filter "action:vs*"
        defines "_CRT_SECURE_NO_WARNINGS"
        staticruntime "on"

    -- Linux and GCC/Clang settings
    filter "system:linux or toolset:gcc or toolset:clang"
        buildoptions { "-include pch.h" }
    
    filter "files:global/headers.cpp"   
        buildoptions { "/Ycheaders.h" }
    