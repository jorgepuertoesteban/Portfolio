// Created by Jorge Puerto. All Rights Reserved.


#include "GameFramework/JPEGameUserSettings.h"

#include "InputCoreTypes.h"
#include "Kismet/KismetInternationalizationLibrary.h"
#include "Structs/JPEInputSettings.h"

void UJPEGameUserSettings::LoadSettings(bool bForceReload)
{
	Super::LoadSettings(bForceReload);

	Gamma = GEngine->GetDisplayGamma();
	Language = UKismetInternationalizationLibrary::GetCurrentLanguage();
	for (auto& [Action, Key] : InputKeysByActionTraditional)
	{
		Key = UJPEInputUtils::GetInputKeyFromTraditionalInputSettings(Action);
	}
	for (auto& [Action, Sensitivity] : InputSensitivitiesByActionTraditional)
	{
		Sensitivity = UJPEInputUtils::GetInputSensitivityFromTraditionalInputSettings(Action);
	}
	for (auto& [Action, bInvert] : InputInvertsByActionTraditional)
	{
		bInvert = UJPEInputUtils::GetInputInvertFromTraditionalInputSettings(Action);
	}
	for (auto& [Action, DeadZone] : InputDeadZonesByActionTraditional)
	{
		DeadZone = UJPEInputUtils::GetInputDeadZoneFromTraditionalInputSettings(Action);
	}
	for (auto& [Action, Key] : InputKeysByActionEnhanced)
	{
		Key = UJPEInputUtils::GetInputKeyFromEnhancedInputSettings(Action);
	}
	for (auto& [Action, Sensitivity] : InputSensitivitiesByActionEnhanced)
	{
		Sensitivity = UJPEInputUtils::GetInputSensitivityFromEnhancedInputSettings(Action);
	}
	for (auto& [Action, bInvert] : InputInvertsByActionEnhanced)
	{
		bInvert = UJPEInputUtils::GetInputInvertFromEnhancedInputSettings(Action);
	}
	for (auto& [Action, DeadZone] : InputDeadZonesByActionEnhanced)
	{
		DeadZone = UJPEInputUtils::GetInputDeadZoneFromEnhancedInputSettings(Action);
	}

	OnSettingsLoaded.Broadcast();
}

void UJPEGameUserSettings::ApplySettings(bool bCheckForCommandLineOverrides)
{
	Super::ApplySettings(bCheckForCommandLineOverrides);

	if (Gamma != GEngine->GetDisplayGamma())
	{
		GEngine->Exec(GetWorld(), *FString::Printf(TEXT("gamma %f"), Gamma));
	}
	if (Language != UKismetInternationalizationLibrary::GetCurrentLanguage())
	{
		UKismetInternationalizationLibrary::SetCurrentLanguage(Language);
	}
	for (const auto& [Action, Key] : InputKeysByActionTraditional)
	{
		UJPEInputUtils::SaveInputKeyToTraditionalInputSettings(Action, Key);
	}
	for (const auto& [Action, Sensitivity] : InputSensitivitiesByActionTraditional)
	{
		UJPEInputUtils::SaveInputSensitivityToTraditionalInputSettings(Action, Sensitivity);
	}
	for (const auto& [Action, bInvert] : InputInvertsByActionTraditional)
	{
		UJPEInputUtils::SaveInputInvertToTraditionalInputSettings(Action, bInvert);
	}
	for (const auto& [Action, DeadZone] : InputDeadZonesByActionTraditional)
	{
		UJPEInputUtils::SaveInputDeadZoneToTraditionalInputSettings(Action, DeadZone);
	}
	TArray<UInputMappingContext*> InputMappingContexts;
	for (const auto& [Action, Key] : InputKeysByActionEnhanced)
	{
		InputMappingContexts.AddUnique(Action.InputMappingContext);
		UJPEInputUtils::SaveInputKeyToEnhancedInputSettings(Action, Key);
	}
	for (const auto& [Action, Sensitivity] : InputSensitivitiesByActionEnhanced)
	{
		InputMappingContexts.AddUnique(Action.InputMappingContext);
		UJPEInputUtils::SaveInputSensitivityToEnhancedInputSettings(Action, Sensitivity);
	}
	for (const auto& [Action, bInvert] : InputInvertsByActionEnhanced)
	{
		InputMappingContexts.AddUnique(Action.InputMappingContext);
		UJPEInputUtils::SaveInputInvertToEnhancedInputSettings(Action, bInvert);
	}
	for (const auto& [Action, DeadZone] : InputDeadZonesByActionEnhanced)
	{
		InputMappingContexts.AddUnique(Action.InputMappingContext);
		UJPEInputUtils::SaveInputDeadZoneToEnhancedInputSettings(Action, DeadZone);
	}
	UJPEInputUtils::RebuildInput(InputMappingContexts);

	OnSettingsApplied.Broadcast();
}

void UJPEGameUserSettings::SetToDefaults()
{
	Super::SetToDefaults();

	Gamma = 2.2f;
	for (auto& [Action, Key] : InputKeysByActionTraditional)
	{
		const FName ActionName = FName(FString::Printf(TEXT("%s%s"), *Action.Name.ToString(), Action.KeyIndex == 0 ? TEXT("") : TEXT("Alt")));
		GetDefaultInputSettingsWithCallback(ActionName, [&Key](const FJPEInputSettings& DefaultInputSetting)
		{
			Key = DefaultInputSetting.Key;
		});
	}
	for (auto& [Action, Sensitivity] : InputSensitivitiesByActionTraditional)
	{
		const FName ActionName = FName(FString::Printf(TEXT("%s%s"), *Action.Name.ToString(), Action.KeyIndex == 0 ? TEXT("") : TEXT("Alt")));
		GetDefaultInputSettingsWithCallback(ActionName, [&Sensitivity](const FJPEInputSettings& DefaultInputSetting)
		{
			Sensitivity = DefaultInputSetting.Sensitivity;
		});
	}
	for (auto& [Action, bInvert] : InputInvertsByActionTraditional)
	{
		const FName ActionName = FName(FString::Printf(TEXT("%s%s"), *Action.Name.ToString(), Action.KeyIndex == 0 ? TEXT("") : TEXT("Alt")));
		GetDefaultInputSettingsWithCallback(ActionName, [&bInvert](const FJPEInputSettings& DefaultInputSetting)
		{
			bInvert = DefaultInputSetting.bInvert;
		});
	}
	for (auto& [Action, DeadZone] : InputDeadZonesByActionTraditional)
	{
		const FName ActionName = FName(FString::Printf(TEXT("%s%s"), *Action.Name.ToString(), Action.KeyIndex == 0 ? TEXT("") : TEXT("Alt")));
		GetDefaultInputSettingsWithCallback(ActionName, [&DeadZone](const FJPEInputSettings& DefaultInputSetting)
		{
			DeadZone = DefaultInputSetting.DeadZone;
		});
	}
	for (auto& [Action, Key] : InputKeysByActionEnhanced)
	{
		GetDefaultInputSettingsWithCallback(Action.Name, [&Key](const FJPEInputSettings& DefaultInputSetting)
		{
			Key = DefaultInputSetting.Key;
		});
	}
	for (auto& [Action, Sensitivity] : InputSensitivitiesByActionEnhanced)
	{
		GetDefaultInputSettingsWithCallback(Action.Name, [&Sensitivity](const FJPEInputSettings& DefaultInputSetting)
		{
			Sensitivity = DefaultInputSetting.Sensitivity;
		});
	}
	for (auto& [Action, bInvert] : InputInvertsByActionEnhanced)
	{
		GetDefaultInputSettingsWithCallback(Action.Name, [&bInvert](const FJPEInputSettings& DefaultInputSetting)
		{
			bInvert = DefaultInputSetting.bInvert;
		});
	}
	for (auto& [Action, DeadZone] : InputDeadZonesByActionEnhanced)
	{
		GetDefaultInputSettingsWithCallback(Action.Name, [&DeadZone](const FJPEInputSettings& DefaultInputSetting)
		{
			DeadZone = DefaultInputSetting.DeadZone;
		});
	}

	OnSettingsSetToDefaults.Broadcast();
}

UJPEGameUserSettings* UJPEGameUserSettings::GetJPEGameUserSettings()
{
	return Cast<UJPEGameUserSettings>(GetGameUserSettings());
}

void UJPEGameUserSettings::ApplySettingsSafe()
{
	BackupSettings = DuplicateObject<UJPEGameUserSettings>(this, GetTransientPackage());
	BackupSettings->LoadSettings();
	ApplySettings(false);
}

void UJPEGameUserSettings::RevertAppliedSettings()
{
	if (IsValid(BackupSettings))
	{
		GEngine->GameUserSettings->MarkAsGarbage();
		GEngine->GameUserSettings = BackupSettings;
		GEngine->GameUserSettings->ApplySettings(false);
		BackupSettings = nullptr;
	}
}

void UJPEGameUserSettings::KeepAppliedSettings()
{
	if (BackupSettings == nullptr)
	{
		return;
	}
	BackupSettings->MarkAsGarbage();
	BackupSettings = nullptr;
}

void UJPEGameUserSettings::ResetToDefaults()
{
	bIsResettingToDefaults = true;
	SetToDefaults();
	ApplySettingsSafe();
	bIsResettingToDefaults = false;
}

template <typename FunctorType>
void UJPEGameUserSettings::GetDefaultInputSettingsWithCallback(const FName& ActionName, FunctorType&& Callback)
{
	for (const FJPEInputSettings& DefaultInputSetting : DefaultInputSettings)
	{
		if (ActionName.IsEqual(DefaultInputSetting.ActionName))
		{
			Callback(DefaultInputSetting);
			break;
		}
	}
}

FMappableActionTraditional UJPEGameUserSettings::GetActionForKeyTraditional(const FKey& Key) const
{
	for (TTuple<FMappableActionTraditional, FKey> ActionAndKeyTraditional : InputKeysByActionTraditional)
	{
		if (ActionAndKeyTraditional.Value == Key)
		{
			return ActionAndKeyTraditional.Key;
		}
	}
	return FMappableActionTraditional();
}

FKey UJPEGameUserSettings::GetInputKeyForActionTraditional(const FMappableActionTraditional& Action)
{
	if (!InputKeysByActionTraditional.Find(Action))
	{
		InputKeysByActionTraditional.Add(Action, UJPEInputUtils::GetInputKeyFromTraditionalInputSettings(Action));
	}
	return InputKeysByActionTraditional[Action];
}

void UJPEGameUserSettings::SetInputKeyForActionTraditional(const FMappableActionTraditional& Action, const FKey& NewKey)
{
	if (InputKeysByActionTraditional.Find(Action))
	{
		InputKeysByActionTraditional[Action] = NewKey;
	}
	else
	{
		InputKeysByActionTraditional.Add(Action, NewKey);
	}
}

float UJPEGameUserSettings::GetInputSensitivityTraditional(const FMappableActionTraditional& Action)
{
	if (!InputSensitivitiesByActionTraditional.Find(Action))
	{
		InputSensitivitiesByActionTraditional.Add(Action, UJPEInputUtils::GetInputSensitivityFromTraditionalInputSettings(Action));
	}
	return InputSensitivitiesByActionTraditional[Action];
}

void UJPEGameUserSettings::SetInputSensitivityTraditional(const FMappableActionTraditional& Action, const float NewSensitivity)
{
	if (InputSensitivitiesByActionTraditional.Find(Action))
	{
		InputSensitivitiesByActionTraditional[Action] = NewSensitivity;
	}
	else
	{
		InputSensitivitiesByActionTraditional.Add(Action, NewSensitivity);
	}
}

bool UJPEGameUserSettings::GetInputInvertTraditional(const FMappableActionTraditional& Action)
{
	if (!InputInvertsByActionTraditional.Find(Action))
	{
		InputInvertsByActionTraditional.Add(Action, UJPEInputUtils::GetInputInvertFromTraditionalInputSettings(Action));
	}
	return InputInvertsByActionTraditional[Action];
}

void UJPEGameUserSettings::SetInputInvertTraditional(const FMappableActionTraditional& Action, const bool bNewInvert)
{
	if (InputInvertsByActionTraditional.Find(Action))
	{
		InputInvertsByActionTraditional[Action] = bNewInvert;
	}
	else
	{
		InputInvertsByActionTraditional.Add(Action, bNewInvert);
	}
}

float UJPEGameUserSettings::GetInputDeadZoneTraditional(const FMappableActionTraditional& Action)
{
	if (!InputDeadZonesByActionTraditional.Find(Action))
	{
		InputDeadZonesByActionTraditional.Add(Action, UJPEInputUtils::GetInputDeadZoneFromTraditionalInputSettings(Action));
	}
	return InputDeadZonesByActionTraditional[Action];
}

void UJPEGameUserSettings::SetInputDeadZoneTraditional(const FMappableActionTraditional& Action, const float NewDeadZone)
{
	if (InputDeadZonesByActionTraditional.Find(Action))
	{
		InputDeadZonesByActionTraditional[Action] = NewDeadZone;
	}
	else
	{
		InputDeadZonesByActionTraditional.Add(Action, NewDeadZone);
	}
}

FMappableActionEnhanced UJPEGameUserSettings::GetActionForKeyEnhanced(const FKey& Key) const
{
	for (TTuple<FMappableActionEnhanced, FKey> ActionAndKeyEnhanced : InputKeysByActionEnhanced)
	{
		if (ActionAndKeyEnhanced.Value == Key)
		{
			return ActionAndKeyEnhanced.Key;
		}
	}
	return FMappableActionEnhanced();
}

FKey UJPEGameUserSettings::GetInputKeyForActionEnhanced(const FMappableActionEnhanced& Action)
{
	if (!InputKeysByActionEnhanced.Find(Action))
	{
		InputKeysByActionEnhanced.Add(Action, UJPEInputUtils::GetInputKeyFromEnhancedInputSettings(Action));
	}
	return InputKeysByActionEnhanced[Action];
}

void UJPEGameUserSettings::SetInputKeyForActionEnhanced(const FMappableActionEnhanced& Action, const FKey& NewKey)
{
	if (InputKeysByActionEnhanced.Find(Action))
	{
		InputKeysByActionEnhanced[Action] = NewKey;
	}
	else
	{
		InputKeysByActionEnhanced.Add(Action, NewKey);
	}
}

float UJPEGameUserSettings::GetInputSensitivityEnhanced(const FMappableActionEnhanced& Action)
{
	if (!InputSensitivitiesByActionEnhanced.Find(Action))
	{
		InputSensitivitiesByActionEnhanced.Add(Action, UJPEInputUtils::GetInputSensitivityFromEnhancedInputSettings(Action));
	}
	return InputSensitivitiesByActionEnhanced[Action];
}

void UJPEGameUserSettings::SetInputSensitivityEnhanced(const FMappableActionEnhanced& Action, const float NewSensitivity)
{
	if (InputSensitivitiesByActionEnhanced.Find(Action))
	{
		InputSensitivitiesByActionEnhanced[Action] = NewSensitivity;
	}
	else
	{
		InputSensitivitiesByActionEnhanced.Add(Action, NewSensitivity);
	}
}

bool UJPEGameUserSettings::GetInputInvertEnhanced(const FMappableActionEnhanced& Action)
{
	if (!InputInvertsByActionEnhanced.Find(Action))
	{
		InputInvertsByActionEnhanced.Add(Action, UJPEInputUtils::GetInputInvertFromEnhancedInputSettings(Action));
	}
	return InputInvertsByActionEnhanced[Action];
}

void UJPEGameUserSettings::SetInputInvertEnhanced(const FMappableActionEnhanced& Action, const bool bNewInvert)
{
	if (InputInvertsByActionEnhanced.Find(Action))
	{
		InputInvertsByActionEnhanced[Action] = bNewInvert;
	}
	else
	{
		InputInvertsByActionEnhanced.Add(Action, bNewInvert);
	}
}

float UJPEGameUserSettings::GetInputDeadZoneEnhanced(const FMappableActionEnhanced& Action)
{
	if (!InputDeadZonesByActionEnhanced.Find(Action))
	{
		InputDeadZonesByActionEnhanced.Add(Action, UJPEInputUtils::GetInputDeadZoneFromEnhancedInputSettings(Action));
	}
	return InputDeadZonesByActionEnhanced[Action];
}

void UJPEGameUserSettings::SetInputDeadZoneEnhanced(const FMappableActionEnhanced& Action, const float NewDeadZone)
{
	if (InputDeadZonesByActionEnhanced.Find(Action))
	{
		InputDeadZonesByActionEnhanced[Action] = NewDeadZone;
	}
	else
	{
		InputDeadZonesByActionEnhanced.Add(Action, NewDeadZone);
	}
}
