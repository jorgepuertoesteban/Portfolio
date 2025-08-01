// Created by Jorge Puerto. All Rights Reserved.


#include "Settings/Items/SettingsWidget_FoliageQuality.h"

#include "GameFramework/JPEGameUserSettings.h"

FString USettingsWidget_FoliageQuality::GetCurrentSettingValue() const
{
	FString Result = "";
	if (GEngine && GEngine->GameUserSettings)
	{
		Result = QualityLevelToString(GEngine->GameUserSettings->GetFoliageQuality());
	}
	return Result;
}

void USettingsWidget_FoliageQuality::UpdateSetting(const int32 NewValue) const
{
	if (0 <= NewValue && NewValue < NumAvailableQualities)
	{
		GEngine->GameUserSettings->SetFoliageQuality(NewValue);
		if (const UJPEGameUserSettings* Settings = UJPEGameUserSettings::GetJPEGameUserSettings())
		{
			Settings->OnQualitySettingsEdited.Broadcast();
		}
	}
}