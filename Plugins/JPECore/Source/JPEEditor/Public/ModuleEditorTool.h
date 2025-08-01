// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "ProjectModuleData.h"
#include "UObject/NoExportTypes.h"
#include "ModuleEditorTool.generated.h"


UENUM(BlueprintType)
enum class EModuleLoadingPhase : uint8
{
	EarliestPossible		UMETA(DisplayName = "EarliestPossible"),
	PreEarlyLoadingScreen	UMETA(DisplayName = "PreEarlyLoadingScreen"),
	PreLoadingScreen		UMETA(DisplayName = "PreLoadingScreen"),
	PreDefault				UMETA(DisplayName = "PreDefault"),
	Default					UMETA(DisplayName = "Default"),
	PostDefault				UMETA(DisplayName = "PostDefault"),
	PostEngineInit			UMETA(DisplayName = "PostEngineInit"),
	None					UMETA(DisplayName = "None"),
	Max						UMETA(DisplayName = "Max")
};

UENUM(BlueprintType)
enum class ECPPModuleType : uint8
{
	Runtime				UMETA(DisplayName = "Runtime"),
	RuntimeNoCommandlet	UMETA(DisplayName = "RuntimeNoCommandlet"),
	Developer			UMETA(DisplayName = "Developer"),
	Editor				UMETA(DisplayName = "Editor"),
	EditorNoCommandlet	UMETA(DisplayName = "EditorNoCommandlet")
};

UENUM(BlueprintType)
enum class EModuleTemplates : uint8
{
	Custom		UMETA(DisplayName = "Custom"),
	Editor		UMETA(DisplayName = "Editor"),
	Runtime		UMETA(DisplayName = "Runtime")
};


UCLASS()
class JPEEDITOR_API UModuleEditorTool : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category="New Module")
	bool AddToPlugin = false;
	FString PluginName = "ExamplePlugin";

	
	/** The name of the new module which will be created */
	UPROPERTY(EditAnywhere, Category = "New Module")
		FString NewModuleName = "";
	/** Use a module preset. Selected custom for full control */
	UPROPERTY(EditAnywhere, Category = "New Module")
		EModuleTemplates ModuleTemplate = EModuleTemplates::Runtime;
	/** The type of module to be created. */
	UPROPERTY(EditAnywhere, Category = "New Module", meta = (EditCondition = "ModuleTemplate == EModuleTemplates::Custom"))
		ECPPModuleType ModuleType = ECPPModuleType::Runtime;
	/** When should the engine try to load this module */
	UPROPERTY(EditAnywhere, Category = "New Module", meta = (EditCondition = "ModuleTemplate == EModuleTemplates::Custom"))
		EModuleLoadingPhase LoadingPhase = EModuleLoadingPhase::Default;

	bool ValidateInput(FString& ErrorMessage);
	//Generate all files requited for a module
	void GenerateModule();
private:
	//Take a UPlugin or UProject String and injects JSON into the modules list
	bool InjectModuleData(FString& UProjectOrPluginString, FProjectModuleData NewModule);
	//Takes the path of a target and injects a ModuleNames into its constructor
	void InjectExtraModuleNames(FString TargetPath, FString ExtraModuleName);
	
	bool NameContainsOnlyLegalCharacters(const FString& ModuleName, FString& OutIllegalCharacters);
};
