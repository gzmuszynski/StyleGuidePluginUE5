// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class StyleGuideEditor : ModuleRules
{
	public StyleGuideEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		PrivateIncludePaths.Add(Path.Combine(GetModuleDirectory("PropertyEditor"), "Private"));
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"DataValidation",
				"DeveloperSettings",
				"DetailCustomizations",
				"PropertyEditor",
				"UnrealEd",
				"Slate",
				"SlateCore",
				"EditorInteractiveToolsFramework",
				"InterchangeCore",
				"InterchangeEngine",
				"StyleGuideCore",
				"StyleGuideModules"
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"PropertyEditor", 
				"AssetManagerEditor"
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"PropertyEditor"
			});
	}
}
