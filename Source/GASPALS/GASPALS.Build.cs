
using System.IO;
using UnrealBuildTool;
using UnrealBuildTool.Rules;

public class GASPALS : ModuleRules
{
	public GASPALS(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        CppStandard = CppStandardVersion.Cpp20;
        bEnableExceptions = true;
        PublicDependencyModuleNames.AddRange(new string[] {
    "Core",
    "CoreUObject",
    "Engine",
    "InputCore",
    "RHI",
    "RenderCore",
    "Media",
    "MediaAssets",
    "OpenCV",
    "OpenCVHelper",
    "UMG",
    "Slate",
    "SlateCore",
    "CinematicCamera",
    "LevelSequence",
    "MovieScene",
    "MovieSceneTracks",
    "AssetRegistry",
            "Json",        
    "JsonUtilities" });


        PrivateDependencyModuleNames.AddRange(new string[] {
        "OpenCV",
        "RHI", 
        "RenderCore", 
		"Core",
        "Renderer",
        "CoreUObject",
        "Engine",
        "InputCore",
        "UMG",
        "Slate",
        "SlateCore",
        "D3D12RHI",
        "Media",
        "MediaAssets",
        "UMG",
        "Slate",
        "SlateCore",
         "RHI",
        "RenderCore",
            "Json",          
    "JsonUtilities"
        });
        
    }
}
