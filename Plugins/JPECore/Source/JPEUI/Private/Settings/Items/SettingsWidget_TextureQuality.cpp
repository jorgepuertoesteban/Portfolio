// Created by Jorge Puerto. All Rights Reserved.


#include "Settings/Items/SettingsWidget_TextureQuality.h"

#include "GameFramework/JPEGameUserSettings.h"

FString USettingsWidget_TextureQuality::GetCurrentSettingValue() const
{
	FString Result = "";
	if (GEngine && GEngine->GameUserSettings)
	{
		Result = QualityLevelToString(GEngine->GameUserSettings->GetTextureQuality());
	}
	return Result;
}

void USettingsWidget_TextureQuality::UpdateSetting(const int32 NewValue) const
{
	if (0 <= NewValue && NewValue < NumAvailableQualities)
	{
		GEngine->GameUserSettings->SetTextureQuality(NewValue);
		if (const UJPEGameUserSettings* Settings = UJPEGameUserSettings::GetJPEGameUserSettings())
		{
			Settings->OnQualitySettingsEdited.Broadcast();
		}
	}
}