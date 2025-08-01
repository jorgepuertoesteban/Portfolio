// Created by Jorge Puerto. All Rights Reserved.


#include "Settings/Items/SettingsWidget_Language.h"

#include "GameFramework/JPEGameUserSettings.h"

#define LOCTEXT_NAMESPACE "SettingsWidget"

void USettingsWidget_Language::GetComboBoxOptions(TArray<FString>& OutOptions)
{
	Languages.GenerateValueArray(OutOptions);
}

FString USettingsWidget_Language::GetCurrentSettingValue() const
{
	FString Result = LOCTEXT("Undefined", "Undefined").ToString();
	if (const UJPEGameUserSettings* Settings = UJPEGameUserSettings::GetJPEGameUserSettings())
	{
		if (const FString* LanguageString = Languages.Find(Settings->GetLanguage()))
		{
			Result = *LanguageString;
		}
	}
	return Result;
}

void USettingsWidget_Language::UpdateSetting(const int32 NewValue) const
{
	if (UJPEGameUserSettings* Settings = UJPEGameUserSettings::GetJPEGameUserSettings())
	{
		TArray<FString> LanguageKeys;
		Languages.GetKeys(LanguageKeys);
		if (LanguageKeys.IsValidIndex(NewValue))
		{
			Settings->SetLanguage(LanguageKeys[NewValue]);
		}
	}
}

#undef LOCTEXT_NAMESPACE
