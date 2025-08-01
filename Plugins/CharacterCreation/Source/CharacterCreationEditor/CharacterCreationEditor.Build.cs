// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CharacterCreationEditor : ModuleRules
{
	public CharacterCreationEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
            }
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
                "CharacterCreationEditor/Public",
				"CharacterCreationEditor/Private"
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
                "UnrealEd",
                "CharacterCreation",
				"GeometryCore",
                "GeometryFramework",
                "Persona",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"InputCore",
				"AnimGraph",
				"PropertyEditor",
				"LevelEditor",
				"WorkspaceMenuStructure",
				"EditorStyle",
				"ToolMenus",
				"RenderCore",
				"RHI",
				"MeshConversion",
				"ApplicationCore",
				"Projects",
				"MeshDescription",
				"StaticMeshDescription",
				"MeshDescriptionOperations",
				"ViewportInteraction",
				"UMG",
				"AssetTools",
				"AssetRegistry",
				"Settings",
				"InteractiveToolsFramework",
				"EditorInteractiveToolsFramework",
				"MeshModelingTools",
				"MeshModelingToolsEditorOnly",
				"ViewportInteraction",
				"DynamicMesh",
				"ModelingComponents",
				"AdvancedPreviewScene",
				"MeshUtilities",
				"ContentBrowser",
				"RawMesh",
				"SkeletalMeshEditor",
                "DataValidation",
                "AssetDefinition",
				"HairStrandsCore",
                "AdvancedPreviewScene"
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
