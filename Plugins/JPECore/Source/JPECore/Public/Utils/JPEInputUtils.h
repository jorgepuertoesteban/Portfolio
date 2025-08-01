// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Structs/MappableActionEnhanced.h"
#include "Structs/MappableActionTraditional.h"
#include "JPEInputUtils.generated.h"

UCLASS()
class JPECORE_API UJPEInputUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	static FText GetDisplayNameForActionTraditional(const FMappableActionTraditional& Action);
	UFUNCTION(BlueprintCallable)
	static FKey GetInputKeyFromTraditionalInputSettings(const FMappableActionTraditional& Action);
	UFUNCTION(BlueprintCallable)
	static void SaveInputKeyToTraditionalInputSettings(const FMappableActionTraditional& Action, const FKey& NewInputKey);

	UFUNCTION(BlueprintCallable)
	static float GetInputSensitivityFromTraditionalInputSettings(const FMappableActionTraditional& Action);
	UFUNCTION(BlueprintCallable)
	static void SaveInputSensitivityToTraditionalInputSettings(const FMappableActionTraditional& Action, const float NewSensitivity);

	UFUNCTION(BlueprintCallable)
	static bool GetInputInvertFromTraditionalInputSettings(const FMappableActionTraditional& Action);
	UFUNCTION(BlueprintCallable)
	static void SaveInputInvertToTraditionalInputSettings(const FMappableActionTraditional& Action, const bool bNewInvert);

	UFUNCTION(BlueprintCallable)
	static float GetInputDeadZoneFromTraditionalInputSettings(const FMappableActionTraditional& Action);
	UFUNCTION(BlueprintCallable)
	static void SaveInputDeadZoneToTraditionalInputSettings(const FMappableActionTraditional& Action, const float NewDeadZone);

	UFUNCTION(BlueprintCallable)
	static FText GetDisplayNameForActionEnhanced(const FMappableActionEnhanced& Action);
	UFUNCTION(BlueprintCallable)
	static FKey GetInputKeyFromEnhancedInputSettings(const FMappableActionEnhanced& Action);
	UFUNCTION(BlueprintCallable)
	static void SaveInputKeyToEnhancedInputSettings(const FMappableActionEnhanced& Action, const FKey& NewInputKey);

	UFUNCTION(BlueprintCallable)
	static float GetInputSensitivityFromEnhancedInputSettings(const FMappableActionEnhanced& Action);
	UFUNCTION(BlueprintCallable)
	static void SaveInputSensitivityToEnhancedInputSettings(const FMappableActionEnhanced& Action, const float NewSensitivity);

	UFUNCTION(BlueprintCallable)
	static bool GetInputInvertFromEnhancedInputSettings(const FMappableActionEnhanced& Action);
	UFUNCTION(BlueprintCallable)
	static void SaveInputInvertToEnhancedInputSettings(const FMappableActionEnhanced& Action, const bool bNewInvert);

	UFUNCTION(BlueprintCallable)
	static float GetInputDeadZoneFromEnhancedInputSettings(const FMappableActionEnhanced& Action);
	UFUNCTION(BlueprintCallable)
	static void SaveInputDeadZoneToEnhancedInputSettings(const FMappableActionEnhanced& Action, const float NewDeadZone);

	UFUNCTION(BlueprintCallable)
	static void RebuildInput(const TArray<UInputMappingContext*>& InputMappingContexts);

};