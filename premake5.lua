workspace "ct-fat"
configurations { "debug", "release" }
language "C"
includedirs { "ext", "src", "../geom-c/src" }
targetdir "bin/%{cfg.buildcfg}"
flags { "Symbols", "C++11" }
linkoptions {"-lm"}

filter "configurations:debug"
defines { "DEBUG", "CT_FEATURE_CHECKS", "CT_FEATURE_CHECK_MEM" }

filter "configurations:release"
defines { "NDEBUG", "CT_FEATURE_LOG" }
optimize "Size"

----- test

project "test"
kind "ConsoleApp"
files { "test/**.c" }
links "lib"
dependson "lib"
defines { "CT_FEATURE_ANSI" }
flags { "FatalWarnings", "LinkTimeOptimization" }

----- lib

project "lib"
files { "src/**.h", "src/**.c" }
kind "StaticLib"
targetname "ctfat"
