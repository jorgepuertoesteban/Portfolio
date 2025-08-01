// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class JPEEditor : ModuleRules
{
	public JPEEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(
			new string[] {
				System.IO.Path.GetFullPath(Target.RelativeEnginePath) + "Source/Editor/Blutility/Private",
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
				"CoreUObject",
				"Engine",
				"InputCore",
				"Blutility",
				"UMG",
				"EditorScriptingUtilities",
				"UMGEditor",
				"UnrealEd",
				"WorkspaceMenuStructure",
				"Slate",
				"SlateCore",
				"ApplicationCore",
				"EditorFramework",
				"ImageWrapper",
			}
			);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
                "JsonUtilities",
				"Json",
				"ToolMenus",
				"EditorStyle",
				"Projects",
				"GameProjectGeneration",
				"DesktopPlatform",
				"AppFramework",
				"EngineSettings",
				"PropertyEditor",
				"PropertyPath",
				"ScriptableEditorWidgets",
				"AssetTools",
				"DeveloperSettings",
				"PluginBrowser",
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
