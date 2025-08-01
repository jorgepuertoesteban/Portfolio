// Created by Jorge Puerto. All Rights Reserved.


#include "Settings/Items/SettingsWidget_VSync.h"

#include "GameFramework/GameUserSettings.h"

bool USettingsWidget_VSync::GetCurrentSettingValue() const
{
	bool Result = false;
	if (GEngine && GEngine->GameUserSettings)
	{
		Result = GEngine->GameUserSettings->IsVSyncEnabled();
	}
	return Result;
}

void USettingsWidget_VSync::UpdateSetting(const bool bNewValue) const
{
	GEngine->GameUserSettings->SetVSyncEnabled(bNewValue);
}