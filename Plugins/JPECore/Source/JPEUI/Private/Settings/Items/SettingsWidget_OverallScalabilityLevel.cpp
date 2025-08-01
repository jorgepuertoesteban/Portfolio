// Created by Jorge Puerto. All Rights Reserved.


#include "Settings/Items/SettingsWidget_OverallScalabilityLevel.h"

#include "GameFramework/JPEGameUserSettings.h"

FString USettingsWidget_OverallScalabilityLevel::GetCurrentSettingValue() const
{
	FString Result = "";
	if (GEngine && GEngine->GameUserSettings)
	{
		Result = QualityLevelToString(GEngine->GameUserSettings->GetOverallScalabilityLevel());
	}
	return Result;
}

void USettingsWidget_OverallScalabilityLevel::UpdateSetting(const int32 NewValue) const
{
	if (0 <= NewValue && NewValue < NumAvailableQualities)
	{
		GEngine->GameUserSettings->SetOverallScalabilityLevel(NewValue);
		if (const UJPEGameUserSettings* Settings = UJPEGameUserSettings::GetJPEGameUserSettings())
		{
			Settings->OnQualitySettingsEdited.Broadcast();
		}
	}
}