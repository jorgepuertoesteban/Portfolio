// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TemplateSettings.generated.h"

USTRUCT()
struct FExtenstionPluginsTemplateData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = PluginTemplate, meta = (RelativePath))
	FString Path;

	UPROPERTY(EditAnywhere, Category = PluginTemplate)
	FText Label;

	UPROPERTY(EditAnywhere, Category = PluginTemplate)
	FText Description;

	UPROPERTY(config, EditAnywhere, Category = Plugins)
	bool bIsEnabledByDefault = false;
};

/**
 * 
 */
UCLASS(config = Editor, defaultconfig)
class UTemplateSettings : public UDeveloperSettings
{
	GENERATED_UCLASS_BODY()
public:
	UPROPERTY(config, EditAnywhere, Category = Plugins)
	TArray<FExtenstionPluginsTemplateData> PluginTemplates;
};
