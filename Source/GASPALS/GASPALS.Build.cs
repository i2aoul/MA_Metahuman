// Fill out your copyright notice in the Description page of Project Settings.

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
            "Json",           // <-- hier
    "JsonUtilities" });


        PrivateDependencyModuleNames.AddRange(new string[] {
		// You can list dependencies that should only be used privately in this module
        "OpenCV", // If you have an OpenCV module setup, add it here
        "RHI",    // Example of other private dependencies
        "RenderCore",  // Another example
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
            "Json",           // <-- hier
    "JsonUtilities"
        });
        
    }
}
