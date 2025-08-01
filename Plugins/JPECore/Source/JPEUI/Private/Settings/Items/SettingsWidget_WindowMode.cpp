// Created by Jorge Puerto. All Rights Reserved.


#include "Settings/Items/SettingsWidget_WindowMode.h"

#include "GameFramework/GameUserSettings.h"

bool USettingsWidget_WindowMode::Initialize()
{
	WindowModes.Empty();
	for (int32 i = 0; i < EWindowMode::NumWindowModes; ++i)
	{
		WindowModes.Add(WindowModeToString(EWindowMode::ConvertIntToWindowMode(i)));
	}

	return Super::Initialize();
}

FString USettingsWidget_WindowMode::WindowModeToString(const EWindowMode::Type WindowMode) const
{
	if (WindowModeTextsByType.Contains(WindowMode))
	{
		return WindowModeTextsByType[WindowMode].ToString();
	}
	return "";
}

void USettingsWidget_WindowMode::GetCarouselOptions(TArray<FString>& OutOptions)
{
	OutOptions = WindowModes;
}

FString USettingsWidget_WindowMode::GetCurrentSettingValue() const
{
	FString Result = "";
	if (GEngine && GEngine->GameUserSettings)
	{
		Result = WindowModeToString(GEngine->GameUserSettings->GetFullscreenMode());
	}
	return Result;
}

void USettingsWidget_WindowMode::UpdateSetting(const int32 NewValue) const
{
	GEngine->GameUserSettings->SetFullscreenMode(EWindowMode::ConvertIntToWindowMode(NewValue));
}