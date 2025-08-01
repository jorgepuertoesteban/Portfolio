// Created by Jorge Puerto. All Rights Reserved.

using UnrealBuildTool;

public class JPEUIEditor : ModuleRules
{
	public JPEUIEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		var forceCast = System.Environment.GetEnvironmentVariable("JPE_FORCE_CAST");
		if (!string.IsNullOrEmpty(forceCast) && forceCast == "1")
		{
			PublicDefinitions.Add("JPE_FORCE_CAST=1");
			System.Console.WriteLine("JPE_FORCE_CAST is defined.");
		}

		PrivateIncludePaths.AddRange(new string[]
		{
			"JPEUIEditor/Public",
			"JPEUIEditor/Private"
		});

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"JPEUI",
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"CoreUObject",
			"Engine",
			"Slate",
			"SlateCore",
			"UnrealEd",
		});
	}
}