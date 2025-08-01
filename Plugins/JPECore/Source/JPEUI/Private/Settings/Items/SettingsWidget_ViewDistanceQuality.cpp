// Created by Jorge Puerto. All Rights Reserved.


#include "Settings/Items/SettingsWidget_ViewDistanceQuality.h"

#include "GameFramework/JPEGameUserSettings.h"

#define LOCTEXT_NAMESPACE "SettingsWidget"

FString USettingsWidget_ViewDistanceQuality::GetCurrentSettingValue() const
{
	FString Result = "";
	if (GEngine && GEngine->GameUserSettings)
	{
		Result = QualityLevelToString(GEngine->GameUserSettings->GetViewDistanceQuality());
	}
	return Result;
}

void USettingsWidget_ViewDistanceQuality::UpdateSetting(const int32 NewValue) const
{
	if (0 <= NewValue && NewValue < NumAvailableQualities)
	{
		GEngine->GameUserSettings->SetViewDistanceQuality(NewValue);
		if (const UJPEGameUserSettings* Settings = UJPEGameUserSettings::GetJPEGameUserSettings())
		{
			Settings->OnQualitySettingsEdited.Broadcast();
		}
	}
}

FString USettingsWidget_ViewDistanceQuality::QualityLevelToString(const int32 QualityLevel) const
{
	switch (QualityLevel)
	{
	case INDEX_NONE:
		return LOCTEXT("Custom", "Custom").ToString();
	case 0:
		return LOCTEXT("ViewDistanceNear", "Near").ToString();
	case 1:
		return LOCTEXT("ViewDistanceMedium", "Medium").ToString();
	case 2:
		return LOCTEXT("ViewDistanceFar", "Far").ToString();
	case 3:
		return LOCTEXT("ViewDistanceEpic", "Epic").ToString();
	case 4:
		return LOCTEXT("ViewDistanceCinematic", "Cinematic").ToString();
	default:
		break;
	}

	return "";
}

#undef LOCTEXT_NAMESPACE
