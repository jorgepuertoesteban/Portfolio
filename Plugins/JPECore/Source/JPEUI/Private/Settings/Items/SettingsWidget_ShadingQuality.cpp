// Created by Jorge Puerto. All Rights Reserved.


#include "Settings/Items/SettingsWidget_ShadingQuality.h"

#include "GameFramework/JPEGameUserSettings.h"

FString USettingsWidget_ShadingQuality::GetCurrentSettingValue() const
{
	FString Result = "";
	if (GEngine && GEngine->GameUserSettings)
	{
		Result = QualityLevelToString(GEngine->GameUserSettings->GetShadingQuality());
	}
	return Result;
}

void USettingsWidget_ShadingQuality::UpdateSetting(const int32 NewValue) const
{
	if (0 <= NewValue && NewValue < NumAvailableQualities)
	{
		GEngine->GameUserSettings->SetShadingQuality(NewValue);
		if (const UJPEGameUserSettings* Settings = UJPEGameUserSettings::GetJPEGameUserSettings())
		{
			Settings->OnQualitySettingsEdited.Broadcast();
		}
	}
}