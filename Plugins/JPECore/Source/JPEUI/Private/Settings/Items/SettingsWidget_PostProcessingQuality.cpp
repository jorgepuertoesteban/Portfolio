// Created by Jorge Puerto. All Rights Reserved.


#include "Settings/Items/SettingsWidget_PostProcessingQuality.h"

#include "GameFramework/JPEGameUserSettings.h"

FString USettingsWidget_PostProcessingQuality::GetCurrentSettingValue() const
{
	FString Result = "";
	if (GEngine && GEngine->GameUserSettings)
	{
		Result = QualityLevelToString(GEngine->GameUserSettings->GetPostProcessingQuality());
	}
	return Result;
}

void USettingsWidget_PostProcessingQuality::UpdateSetting(const int32 NewValue) const
{
	if (0 <= NewValue && NewValue < NumAvailableQualities)
	{
		GEngine->GameUserSettings->SetPostProcessingQuality(NewValue);
		if (const UJPEGameUserSettings* Settings = UJPEGameUserSettings::GetJPEGameUserSettings())
		{
			Settings->OnQualitySettingsEdited.Broadcast();
		}
	}
}