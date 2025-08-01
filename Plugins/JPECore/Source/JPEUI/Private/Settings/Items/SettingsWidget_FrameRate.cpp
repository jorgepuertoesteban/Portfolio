// Created by Jorge Puerto. All Rights Reserved.


#include "Settings/Items/SettingsWidget_FrameRate.h"

#include "GameFramework/GameUserSettings.h"

#define LOCTEXT_NAMESPACE "SettingsWidget"

FString USettingsWidget_FrameRate::FrameRateToString(const int32 FrameRate)
{
	return FrameRate > 0 ? FString::Printf(TEXT("%d%s"), FrameRate, *LOCTEXT("Fps", "fps").ToString()) : LOCTEXT("Unlimited", "Unlimited").ToString();
}

bool USettingsWidget_FrameRate::Initialize()
{
	FrameRates.Empty();
	for (const int32 FrameRate : SupportedFrameRates)
	{
		FrameRates.Add(static_cast<float>(FrameRate), FrameRateToString(FrameRate));
	}
	
	return Super::Initialize();
}

void USettingsWidget_FrameRate::GetCarouselOptions(TArray<FString>& OutOptions)
{
	FrameRates.GenerateValueArray(OutOptions);
}

FString USettingsWidget_FrameRate::GetCurrentSettingValue() const
{
	FString Result = LOCTEXT("Undefined", "Undefined").ToString();
	if (const FString* FontSizeString = FrameRates.Find(GEngine->GameUserSettings->GetFrameRateLimit()))
	{
		Result = *FontSizeString;
	}
	return Result;
}

void USettingsWidget_FrameRate::UpdateSetting(const int32 NewValue) const
{
	if (SupportedFrameRates.IsValidIndex(NewValue))
	{
		GEngine->GameUserSettings->SetFrameRateLimit(SupportedFrameRates[NewValue]);
	}
}

#undef LOCTEXT_NAMESPACE
