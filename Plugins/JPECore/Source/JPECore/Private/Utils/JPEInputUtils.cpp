// Created by Jorge Puerto. All Rights Reserved.

#include "Utils/JPEInputUtils.h"

#include "EnhancedInputLibrary.h"
#include "EnhancedInputModule.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "GameFramework/InputSettings.h"
#include "EnhancedActionKeyMapping.h"
#include "PlayerMappableKeySettings.h"

FText UJPEInputUtils::GetDisplayNameForActionTraditional(const FMappableActionTraditional& Action)
{
	return FText::FromString(FName::NameToDisplayString(Action.Name.ToString(), false));
}

FKey UJPEInputUtils::GetInputKeyFromTraditionalInputSettings(const FMappableActionTraditional& Action)
{
	const UInputSettings* InputSettings = UInputSettings::GetInputSettings();
	if (!InputSettings)
	{
		return EKeys::Invalid;
	}

	TArray<FInputActionKeyMapping> KeyMappings = InputSettings->GetActionMappings().FilterByPredicate([Action](const FInputActionKeyMapping& Mapping)
	{
		return Action.Name == Mapping.ActionName;
	});

	if (!KeyMappings.IsValidIndex(Action.KeyIndex))
	{
		return EKeys::Invalid;
	}

	return KeyMappings[Action.KeyIndex].Key;
}

void UJPEInputUtils::SaveInputKeyToTraditionalInputSettings(const FMappableActionTraditional& Action, const FKey& NewInputKey)
{
	UInputSettings* InputSettings = UInputSettings::GetInputSettings();
	if (!InputSettings)
	{
		return;
	}

	TArray<FInputActionKeyMapping> KeyMappings = InputSettings->GetActionMappings().FilterByPredicate([Action](const FInputActionKeyMapping& Mapping)
	{
		return Action.Name == Mapping.ActionName;
	});

	if (!KeyMappings.IsValidIndex(Action.KeyIndex))
	{
		return;
	}

	for (FInputActionKeyMapping KeyMapping : KeyMappings)
	{
		InputSettings->RemoveActionMapping(KeyMapping);
	}
	KeyMappings[Action.KeyIndex].Key = NewInputKey;
	for (FInputActionKeyMapping KeyMapping : KeyMappings)
	{
		InputSettings->AddActionMapping(KeyMapping);
	}
}

float UJPEInputUtils::GetInputSensitivityFromTraditionalInputSettings(const FMappableActionTraditional& Action)
{
	const UInputSettings* InputSettings = UInputSettings::GetInputSettings();
	if (!InputSettings)
	{
		return 0.f;
	}

	TArray<FInputAxisConfigEntry> AxisConfigs = InputSettings->AxisConfig.FilterByPredicate([Action](const FInputAxisConfigEntry& AxisConfig)
	{
		return Action.Name == AxisConfig.AxisKeyName;
	});

	if (!AxisConfigs.IsValidIndex(0))
	{
		return 0.f;
	}

	return AxisConfigs[0].AxisProperties.Sensitivity;
}

void UJPEInputUtils::SaveInputSensitivityToTraditionalInputSettings(const FMappableActionTraditional& Action, const float NewSensitivity)
{
	UInputSettings* InputSettings = UInputSettings::GetInputSettings();
	if (!InputSettings)
	{
		return;
	}

	for (FInputAxisConfigEntry& AxisConfig : InputSettings->AxisConfig)
	{
		if (Action.Name == AxisConfig.AxisKeyName)
		{
			AxisConfig.AxisProperties.Sensitivity = NewSensitivity;
		}
	}
}

bool UJPEInputUtils::GetInputInvertFromTraditionalInputSettings(const FMappableActionTraditional& Action)
{
	const UInputSettings* InputSettings = UInputSettings::GetInputSettings();
	if (!InputSettings)
	{
		return 0.f;
	}

	TArray<FInputAxisConfigEntry> AxisConfigs = InputSettings->AxisConfig.FilterByPredicate([Action](const FInputAxisConfigEntry& AxisConfig)
	{
		return Action.Name == AxisConfig.AxisKeyName;
	});

	if (!AxisConfigs.IsValidIndex(0))
	{
		return 0.f;
	}

	return AxisConfigs[0].AxisProperties.bInvert;
}

void UJPEInputUtils::SaveInputInvertToTraditionalInputSettings(const FMappableActionTraditional& Action, const bool bNewInvert)
{
	UInputSettings* InputSettings = UInputSettings::GetInputSettings();
	if (!InputSettings)
	{
		return;
	}

	for (FInputAxisConfigEntry& AxisConfig : InputSettings->AxisConfig)
	{
		if (Action.Name == AxisConfig.AxisKeyName)
		{
			AxisConfig.AxisProperties.bInvert = bNewInvert;
		}
	}
}

float UJPEInputUtils::GetInputDeadZoneFromTraditionalInputSettings(const FMappableActionTraditional& Action)
{
	const UInputSettings* InputSettings = UInputSettings::GetInputSettings();
	if (!InputSettings)
	{
		return 0.f;
	}

	TArray<FInputAxisConfigEntry> AxisConfigs = InputSettings->AxisConfig.FilterByPredicate([Action](const FInputAxisConfigEntry& AxisConfig)
	{
		return Action.Name == AxisConfig.AxisKeyName;
	});

	if (!AxisConfigs.IsValidIndex(0))
	{
		return 0.f;
	}

	return AxisConfigs[0].AxisProperties.DeadZone;
}

void UJPEInputUtils::SaveInputDeadZoneToTraditionalInputSettings(const FMappableActionTraditional& Action, const float NewDeadZone)
{
	UInputSettings* InputSettings = UInputSettings::GetInputSettings();
	if (!InputSettings)
	{
		return;
	}

	for (FInputAxisConfigEntry& AxisConfig : InputSettings->AxisConfig)
	{
		if (Action.Name == AxisConfig.AxisKeyName)
		{
			AxisConfig.AxisProperties.DeadZone = NewDeadZone;
		}
	}
}

FText UJPEInputUtils::GetDisplayNameForActionEnhanced(const FMappableActionEnhanced& Action)
{
	if (!Action.InputMappingContext || !Action.InputAction)
	{
		return FText::GetEmpty();
	}

	TArray<FEnhancedActionKeyMapping> Mappings = Action.InputMappingContext->GetMappings().FilterByPredicate([Action](const FEnhancedActionKeyMapping& Mapping)
	{
		const UPlayerMappableKeySettings* PlayerMappableKeySettings = Mapping.GetPlayerMappableKeySettings();
		return PlayerMappableKeySettings && Mapping.Action == Action.InputAction && PlayerMappableKeySettings->Name.IsEqual(Action.Name);
	});

	if (!Mappings.IsValidIndex(0))
	{
		return FText::GetEmpty();
	}

	const UPlayerMappableKeySettings* PlayerMappableKeySettings = Mappings[0].GetPlayerMappableKeySettings();
	if (!PlayerMappableKeySettings)
	{
		return FText::GetEmpty();
	}

	return PlayerMappableKeySettings->DisplayName;
}

FKey UJPEInputUtils::GetInputKeyFromEnhancedInputSettings(const FMappableActionEnhanced& Action)
{
	if (!Action.InputMappingContext || !Action.InputAction)
	{
		return EKeys::Invalid;
	}

	TArray<FEnhancedActionKeyMapping> Mappings = Action.InputMappingContext->GetMappings().FilterByPredicate([Action](const FEnhancedActionKeyMapping& Mapping)
	{
		const UPlayerMappableKeySettings* PlayerMappableKeySettings = Mapping.GetPlayerMappableKeySettings();
		return PlayerMappableKeySettings && Mapping.Action == Action.InputAction && PlayerMappableKeySettings->Name.IsEqual(Action.Name);
	});

	if (!Mappings.IsValidIndex(0))
	{
		return EKeys::Invalid;
	}

	return Mappings[0].Key;
}

void UJPEInputUtils::SaveInputKeyToEnhancedInputSettings(const FMappableActionEnhanced& Action, const FKey& NewInputKey)
{
	if (!Action.InputMappingContext || !Action.InputAction)
	{
		return;
	}

	const TArray<FEnhancedActionKeyMapping>& Mappings = Action.InputMappingContext->GetMappings();
	for (int32 i = 0; i < Mappings.Num(); ++i)
	{
		const UPlayerMappableKeySettings* PlayerMappableKeySettings = Mappings[i].GetPlayerMappableKeySettings();
		if (PlayerMappableKeySettings && Mappings[i].Action == Action.InputAction && PlayerMappableKeySettings->Name.IsEqual(Action.Name))
		{
			Action.InputMappingContext->GetMapping(i).Key = NewInputKey;
			return;
		}
	}
}

float UJPEInputUtils::GetInputSensitivityFromEnhancedInputSettings(const FMappableActionEnhanced& Action)
{
	if (!Action.InputMappingContext || !Action.InputAction)
	{
		return 0.f;
	}

	TArray<FEnhancedActionKeyMapping> Mappings = Action.InputMappingContext->GetMappings().FilterByPredicate([Action](const FEnhancedActionKeyMapping& Mapping)
	{
		const UPlayerMappableKeySettings* PlayerMappableKeySettings = Mapping.GetPlayerMappableKeySettings();
		return PlayerMappableKeySettings && Mapping.Action == Action.InputAction && PlayerMappableKeySettings->Name.IsEqual(Action.Name);
	});

	if (!Mappings.IsValidIndex(0))
	{
		return 0.f;
	}

	for (const TObjectPtr<UInputModifier>& InputModifier : Mappings[0].Modifiers)
	{
		if (const UInputModifierScalar* Modifier = Cast<UInputModifierScalar>(InputModifier.Get()))
		{
			return Modifier->Scalar.X;
		}
	}

	return 0.f;
}

void UJPEInputUtils::SaveInputSensitivityToEnhancedInputSettings(const FMappableActionEnhanced& Action, const float NewSensitivity)
{
	if (!Action.InputMappingContext || !Action.InputAction)
	{
		return;
	}

	const TArray<FEnhancedActionKeyMapping>& Mappings = Action.InputMappingContext->GetMappings();
	for (int32 i = 0; i < Mappings.Num(); ++i)
	{
		const UPlayerMappableKeySettings* PlayerMappableKeySettings = Mappings[i].GetPlayerMappableKeySettings();
		if (PlayerMappableKeySettings && Mappings[i].Action == Action.InputAction && PlayerMappableKeySettings->Name.IsEqual(Action.Name))
		{
			for (int32 j = 0; j < Mappings[i].Modifiers.Num(); ++j)
			{
				if (UInputModifierScalar* Modifier = Cast<UInputModifierScalar>(Mappings[i].Modifiers[j].Get()))
				{
					Modifier->Scalar = FVector(FMath::Abs(NewSensitivity));
					return;
				}
			}
			return;
		}
	}
}

bool UJPEInputUtils::GetInputInvertFromEnhancedInputSettings(const FMappableActionEnhanced& Action)
{
	if (!Action.InputMappingContext || !Action.InputAction)
	{
		return 0.f;
	}

	TArray<FEnhancedActionKeyMapping> Mappings = Action.InputMappingContext->GetMappings().FilterByPredicate([Action](const FEnhancedActionKeyMapping& Mapping)
	{
		const UPlayerMappableKeySettings* PlayerMappableKeySettings = Mapping.GetPlayerMappableKeySettings();
		return PlayerMappableKeySettings && Mapping.Action == Action.InputAction && PlayerMappableKeySettings->Name.IsEqual(Action.Name);
	});

	if (!Mappings.IsValidIndex(0))
	{
		return 0.f;
	}

	for (const TObjectPtr<UInputModifier>& InputModifier : Mappings[0].Modifiers)
	{
		if (const UInputModifierNegate* Modifier = Cast<UInputModifierNegate>(InputModifier.Get()))
		{
			return Modifier->bX;
		}
	}

	return 0.f;
}

void UJPEInputUtils::SaveInputInvertToEnhancedInputSettings(const FMappableActionEnhanced& Action, const bool bNewInvert)
{
	if (!Action.InputMappingContext || !Action.InputAction)
	{
		return;
	}

	const TArray<FEnhancedActionKeyMapping>& Mappings = Action.InputMappingContext->GetMappings();
	for (int32 i = 0; i < Mappings.Num(); ++i)
	{
		const UPlayerMappableKeySettings* PlayerMappableKeySettings = Mappings[i].GetPlayerMappableKeySettings();
		if (PlayerMappableKeySettings && Mappings[i].Action == Action.InputAction && PlayerMappableKeySettings->Name.IsEqual(Action.Name))
		{
			for (int32 j = 0; j < Mappings[i].Modifiers.Num(); ++j)
			{
				if (UInputModifierNegate* Modifier = Cast<UInputModifierNegate>(Mappings[i].Modifiers[j].Get()))
				{
					Modifier->bX = Modifier->bY = Modifier->bZ = bNewInvert;
					return;
				}
			}
			return;
		}
	}
}

float UJPEInputUtils::GetInputDeadZoneFromEnhancedInputSettings(const FMappableActionEnhanced& Action)
{
	if (!Action.InputMappingContext || !Action.InputAction)
	{
		return 0.f;
	}

	TArray<FEnhancedActionKeyMapping> Mappings = Action.InputMappingContext->GetMappings().FilterByPredicate([Action](const FEnhancedActionKeyMapping& Mapping)
	{
		const UPlayerMappableKeySettings* PlayerMappableKeySettings = Mapping.GetPlayerMappableKeySettings();
		return PlayerMappableKeySettings && Mapping.Action == Action.InputAction && PlayerMappableKeySettings->Name.IsEqual(Action.Name);
	});

	if (!Mappings.IsValidIndex(0))
	{
		return 0.f;
	}

	for (const TObjectPtr<UInputModifier>& InputModifier : Mappings[0].Modifiers)
	{
		if (const UInputModifierDeadZone* Modifier = Cast<UInputModifierDeadZone>(InputModifier.Get()))
		{
			return Modifier->LowerThreshold;
		}
	}

	return 0.f;
}

void UJPEInputUtils::SaveInputDeadZoneToEnhancedInputSettings(const FMappableActionEnhanced& Action, const float NewDeadZone)
{
	if (!Action.InputMappingContext || !Action.InputAction)
	{
		return;
	}

	const TArray<FEnhancedActionKeyMapping>& Mappings = Action.InputMappingContext->GetMappings();
	for (int32 i = 0; i < Mappings.Num(); ++i)
	{
		const UPlayerMappableKeySettings* PlayerMappableKeySettings = Mappings[i].GetPlayerMappableKeySettings();
		if (PlayerMappableKeySettings && Mappings[i].Action == Action.InputAction && PlayerMappableKeySettings->Name.IsEqual(Action.Name))
		{
			for (int32 j = 0; j < Mappings[i].Modifiers.Num(); ++j)
			{
				if (UInputModifierDeadZone* Modifier = Cast<UInputModifierDeadZone>(Mappings[i].Modifiers[j].Get()))
				{
					Modifier->LowerThreshold = NewDeadZone;
					return;
				}
			}
			return;
		}
	}
}

void UJPEInputUtils::RebuildInput(const TArray<UInputMappingContext*>& InputMappingContexts)
{
	// Traditional Input
	if (UInputSettings* InputSettings = UInputSettings::GetInputSettings())
	{
		InputSettings->SaveKeyMappings();
		InputSettings->ForceRebuildKeymaps();
	}
	// Enhanced Input
	for (const UInputMappingContext* InputMappingContext : InputMappingContexts)
	{
		IEnhancedInputModule::Get().GetLibrary()->RequestRebuildControlMappingsUsingContext(InputMappingContext);
	}
}
