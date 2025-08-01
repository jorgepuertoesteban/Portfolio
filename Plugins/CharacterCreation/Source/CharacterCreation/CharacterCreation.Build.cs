// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CharacterCreation : ModuleRules
{
	public CharacterCreation(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
	


		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"Engine",
                "JPECore",
				"DynamicMesh",
				"UMG"				
				//"CharacterCreationUI",
				//"MorphToolsRuntime",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Eigen",
				"CoreUObject",
				"Slate",
				"SlateCore",
				"Json",
				"HairStrandsCore",
				"Niagara"
				// ... add private dependencies that you statically link with here ...	
			}
            );
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
