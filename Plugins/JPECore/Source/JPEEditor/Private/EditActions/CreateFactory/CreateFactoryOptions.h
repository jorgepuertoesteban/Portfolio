// Created by Jorge Puerto. All Rights Reserved.

#pragma once

UENUM()
enum class EFactoryType : uint8
{
	Simple = 0,
	Advanced,
	// ...
	Count
};
ENUM_RANGE_BY_COUNT(EFactoryType, EFactoryType::Count)

struct FCreateFactoryOptions
{
	FString ModuleName;
	FString PluginName;
	EFactoryType FactoryType;
	FString AssetNameForTheFactory;
	FString TargetPath;
	bool bReplaceFileIfExists;
};