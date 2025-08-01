// Created by Jorge Puerto. All Rights Reserved.


#include "Settings/Items/SettingsWidget_AntiAliasingQuality.h"

#include "GameFramework/JPEGameUserSettings.h"

FString USettingsWidget_AntiAliasingQuality::GetCurrentSettingValue() const
{
	FString Result = "";
	if (GEngine && GEngine->GameUserSettings)
	{
		Result = QualityLevelToString(GEngine->GameUserSettings->GetAntiAliasingQuality());
	}
	return Result;
}

void USettingsWidget_AntiAliasingQuality::UpdateSetting(const int32 NewValue) const
{
	if (0 <= NewValue && NewValue < NumAvailableQualities)
	{
		GEngine->GameUserSettings->SetAntiAliasingQuality(NewValue);
		if (const UJPEGameUserSettings* Settings = UJPEGameUserSettings::GetJPEGameUserSettings())
		{
			Settings->OnQualitySettingsEdited.Broadcast();
		}
	}
}