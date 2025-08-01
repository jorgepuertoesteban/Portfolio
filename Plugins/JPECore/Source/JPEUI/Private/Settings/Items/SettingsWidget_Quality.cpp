// Created by Jorge Puerto. All Rights Reserved.


#include "Settings/Items/SettingsWidget_Quality.h"

#include "GameFramework/JPEGameUserSettings.h"

#define LOCTEXT_NAMESPACE "SettingsWidget"

bool USettingsWidget_Quality::Initialize()
{
	Qualities.Empty();
	for (int i = 0; i < NumAvailableQualities; ++i)
	{
		Qualities.Add(QualityLevelToString(i));
	}
	
	return Super::Initialize();
}

void USettingsWidget_Quality::NativeConstruct()
{
	Super::NativeConstruct();

	if (UJPEGameUserSettings* Settings = UJPEGameUserSettings::GetJPEGameUserSettings())
	{
		Settings->OnQualitySettingsEdited.AddUniqueDynamic(this, &USettingsWidget_Quality::HandleOnQualitySettingsEdited);
	}
}

void USettingsWidget_Quality::GetCarouselOptions(TArray<FString>& OutOptions)
{
	OutOptions = Qualities;
}

FString USettingsWidget_Quality::QualityLevelToString(const int32 QualityLevel) const
{
	switch (QualityLevel)
	{
	case INDEX_NONE:
		return LOCTEXT("Custom", "Custom").ToString();
	case 0:
		return LOCTEXT("QualityLow", "Low").ToString();
	case 1:
		return LOCTEXT("QualityMedium", "Medium").ToString();
	case 2:
		return LOCTEXT("QualityHigh", "High").ToString();
	case 3:
		return LOCTEXT("QualityEpic", "Epic").ToString();
	case 4:
		return LOCTEXT("QualityCine", "Cinematic").ToString();
	default:
		break;
	}

	return "";
}

void USettingsWidget_Quality::HandleOnQualitySettingsEdited()
{
	UpdateUIValues();
}

#undef LOCTEXT_NAMESPACE
