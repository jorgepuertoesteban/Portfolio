// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class [Module]Editor : ModuleRules
{
	public [Module]Editor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		var forceCast = System.Environment.GetEnvironmentVariable("JPE_FORCE_CAST");
		if (!string.IsNullOrEmpty(forceCast) && forceCast == "1")
		{
			PublicDefinitions.Add("JPE_FORCE_CAST=1");
			System.Console.WriteLine("JPE_FORCE_CAST is defined.");
		}
		
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
            "[Module]"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"CoreUObject",
			"Engine",
			"Slate",
			"SlateCore",
			"UnrealEd"	
		});
	}
}
