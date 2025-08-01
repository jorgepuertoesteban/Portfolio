// Created by Jorge Puerto. All Rights Reserved.


#include "Settings/Items/SettingsWidget_ReflectionQuality.h"

#include "GameFramework/JPEGameUserSettings.h"

FString USettingsWidget_ReflectionQuality::GetCurrentSettingValue() const
{
	FString Result = "";
	if (GEngine && GEngine->GameUserSettings)
	{
		Result = QualityLevelToString(GEngine->GameUserSettings->GetReflectionQuality());
	}
	return Result;
}

void USettingsWidget_ReflectionQuality::UpdateSetting(const int32 NewValue) const
{
	if (0 <= NewValue && NewValue < NumAvailableQualities)
	{
		GEngine->GameUserSettings->SetReflectionQuality(NewValue);
		if (const UJPEGameUserSettings* Settings = UJPEGameUserSettings::GetJPEGameUserSettings())
		{
			Settings->OnQualitySettingsEdited.Broadcast();
		}
	}
}