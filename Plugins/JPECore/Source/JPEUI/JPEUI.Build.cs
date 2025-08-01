// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class JPEUI : ModuleRules
{
	public JPEUI(ReadOnlyTargetRules Target) : base(Target)
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
			"UMG",
			"Engine",
			"JPECore",
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"CoreUObject",
			"Slate",
			"SlateCore",
			"InputCore",
			"RHI"
		});
	}
}