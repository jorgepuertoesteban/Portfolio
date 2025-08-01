// Created by Jorge Puerto. All Rights Reserved.


#include "Settings/Items/SettingsWidget_ScreenResolution.h"

#include "GameFramework/GameUserSettings.h"
#include "Kismet/KismetSystemLibrary.h"

FString USettingsWidget_ScreenResolution::ResolutionToString(const FIntPoint& Resolution)
{
	if (Resolution.X == 0 && Resolution.Y == 0)
	{
		return "Invalid";
	}
	const FIntPoint AspectRatio = Resolution / FMath::GreatestCommonDivisor(Resolution.X, Resolution.Y);
	return FString::Printf(TEXT("%d x %d (%d:%d)"), Resolution.X, Resolution.Y, AspectRatio.X, AspectRatio.Y);
}

bool USettingsWidget_ScreenResolution::Initialize()
{
	TArray<FIntPoint> SupportedResolutions;
	UKismetSystemLibrary::GetSupportedFullscreenResolutions(SupportedResolutions);

	for (int i = SupportedResolutions.Num() - 1; i >= 0; --i)
	{
		Resolutions.Add(SupportedResolutions[i], ResolutionToString(SupportedResolutions[i]));
	}
	
	return Super::Initialize();
}

void USettingsWidget_ScreenResolution::GetComboBoxOptions(TArray<FString>& OutOptions)
{
	Resolutions.GenerateValueArray(OutOptions);
}

FString USettingsWidget_ScreenResolution::GetCurrentSettingValue() const
{
	FString Result = "";
	if (GEngine && GEngine->GameUserSettings)
	{
		Result = ResolutionToString(GEngine->GameUserSettings->GetScreenResolution());
	}
	return Result;
}

void USettingsWidget_ScreenResolution::UpdateSetting(const int32 NewValue) const
{
	TArray<FIntPoint> ResolutionKeys;
	Resolutions.GetKeys(ResolutionKeys);
	if (ResolutionKeys.IsValidIndex(NewValue))
	{
		GEngine->GameUserSettings->SetScreenResolution(ResolutionKeys[NewValue]);
	}
}