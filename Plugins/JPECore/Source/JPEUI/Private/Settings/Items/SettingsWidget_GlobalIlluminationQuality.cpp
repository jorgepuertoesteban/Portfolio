// Created by Jorge Puerto. All Rights Reserved.


#include "Settings/Items/SettingsWidget_GlobalIlluminationQuality.h"

#include "GameFramework/JPEGameUserSettings.h"

FString USettingsWidget_GlobalIlluminationQuality::GetCurrentSettingValue() const
{
	FString Result = "";
	if (GEngine && GEngine->GameUserSettings)
	{
		Result = QualityLevelToString(GEngine->GameUserSettings->GetGlobalIlluminationQuality());
	}
	return Result;
}

void USettingsWidget_GlobalIlluminationQuality::UpdateSetting(const int32 NewValue) const
{
	if (0 <= NewValue && NewValue < NumAvailableQualities)
	{
		GEngine->GameUserSettings->SetGlobalIlluminationQuality(NewValue);
		if (const UJPEGameUserSettings* Settings = UJPEGameUserSettings::GetJPEGameUserSettings())
		{
			Settings->OnQualitySettingsEdited.Broadcast();
		}
	}
}