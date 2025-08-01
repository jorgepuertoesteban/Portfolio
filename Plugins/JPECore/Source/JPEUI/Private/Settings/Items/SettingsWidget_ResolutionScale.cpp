// Created by Jorge Puerto. All Rights Reserved.


#include "Settings/Items/SettingsWidget_ResolutionScale.h"

#include "GameFramework/GameUserSettings.h"

FVector USettingsWidget_ResolutionScale::GetCurrentSettingValue() const
{
	FVector Result = FVector(0.f, 1.f, 0.f);
	if (GEngine && GEngine->GameUserSettings)
	{
		Result.Z = GEngine->GameUserSettings->GetResolutionScaleNormalized();
	}
	return Result;
}

void USettingsWidget_ResolutionScale::UpdateSetting(const float NewValue) const
{
	GEngine->GameUserSettings->SetResolutionScaleNormalized(NewValue);
}