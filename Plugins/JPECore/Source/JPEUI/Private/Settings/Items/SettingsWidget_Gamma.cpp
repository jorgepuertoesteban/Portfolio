// Created by Jorge Puerto. All Rights Reserved.


#include "Settings/Items/SettingsWidget_Gamma.h"

#include "GameFramework/JPEGameUserSettings.h"

FVector USettingsWidget_Gamma::GetCurrentSettingValue() const
{
	FVector Result = FVector(0.5, 5.f, 0.5f);
	if (const UJPEGameUserSettings* Settings = UJPEGameUserSettings::GetJPEGameUserSettings())
	{
		Result.Z = Settings->GetGamma();
	}
	return Result;
}

void USettingsWidget_Gamma::UpdateSetting(const float NewValue) const
{
	if (UJPEGameUserSettings* Settings = UJPEGameUserSettings::GetJPEGameUserSettings())
	{
		Settings->SetGamma(NewValue);
	}
}