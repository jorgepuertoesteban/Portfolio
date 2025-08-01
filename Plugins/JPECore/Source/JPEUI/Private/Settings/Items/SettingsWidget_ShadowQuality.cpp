// Created by Jorge Puerto. All Rights Reserved.


#include "Settings/Items/SettingsWidget_ShadowQuality.h"

#include "GameFramework/JPEGameUserSettings.h"

FString USettingsWidget_ShadowQuality::GetCurrentSettingValue() const
{
	FString Result = "";
	if (GEngine && GEngine->GameUserSettings)
	{
		Result = QualityLevelToString(GEngine->GameUserSettings->GetShadowQuality());
	}
	return Result;
}

void USettingsWidget_ShadowQuality::UpdateSetting(const int32 NewValue) const
{
	if (0 <= NewValue && NewValue < NumAvailableQualities)
	{
		GEngine->GameUserSettings->SetShadowQuality(NewValue);
		if (const UJPEGameUserSettings* Settings = UJPEGameUserSettings::GetJPEGameUserSettings())
		{
			Settings->OnQualitySettingsEdited.Broadcast();
		}
	}
}