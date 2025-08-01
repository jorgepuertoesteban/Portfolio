// Created by Jorge Puerto. All Rights Reserved.


#include "Settings/Items/SettingsWidget_VisualEffectQuality.h"

#include "GameFramework/JPEGameUserSettings.h"

FString USettingsWidget_VisualEffectQuality::GetCurrentSettingValue() const
{
	FString Result = "";
	if (GEngine && GEngine->GameUserSettings)
	{
		Result = QualityLevelToString(GEngine->GameUserSettings->GetVisualEffectQuality());
	}
	return Result;
}

void USettingsWidget_VisualEffectQuality::UpdateSetting(const int32 NewValue) const
{
	if (0 <= NewValue && NewValue < NumAvailableQualities)
	{
		GEngine->GameUserSettings->SetVisualEffectQuality(NewValue);
		if (const UJPEGameUserSettings* Settings = UJPEGameUserSettings::GetJPEGameUserSettings())
		{
			Settings->OnQualitySettingsEdited.Broadcast();
		}
	}
}