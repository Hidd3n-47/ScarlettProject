workspace "Scarlett"
    architecture "x64"
    
    configurations
    {
        "Dev",
        "Release"
    }
    
outputName = "%{cfg.buildcfg}"
    
include "ScarlettEngine/"
include "Scarlett/"
    