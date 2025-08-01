// Created by Jorge Puerto. All Rights Reserved.


#include "Settings/Items/SettingsWidget_InputDeadZone.h"

#include "Components/TextBlock.h"
#include "GameFramework/JPEGameUserSettings.h"

bool USettingsWidget_InputDeadZone::Initialize()
{
	if (Super::Initialize())
	{
		if (!NameLabel) return false;

		if (bUseEnhancedInputSystem)
		{
			NameLabel->SetText(FText::FromString(UJPEInputUtils::GetDisplayNameForActionEnhanced(ActionEnhanced).ToString() +  + " Dead Zone"));
		}
		else
		{
			NameLabel->SetText(FText::FromString(UJPEInputUtils::GetDisplayNameForActionTraditional(ActionTraditional).ToString() +  + " Dead Zone"));
		}
		
		return true;
	}
	return false;
}

FVector USettingsWidget_InputDeadZone::GetCurrentSettingValue() const
{
	FVector Result = FVector(MinValue, MaxValue, MinValue);
	if (UJPEGameUserSettings* Settings = UJPEGameUserSettings::GetJPEGameUserSettings())
	{
		if (bUseEnhancedInputSystem)
		{
			Result.Z = Settings->GetInputDeadZoneEnhanced(ActionEnhanced);
		}
		else
		{
			Result.Z = Settings->GetInputDeadZoneTraditional(ActionTraditional);
		}
	}
	return Result;
}

void USettingsWidget_InputDeadZone::UpdateSetting(const float NewValue) const
{
	if (UJPEGameUserSettings* Settings = UJPEGameUserSettings::GetJPEGameUserSettings())
	{
		if (bUseEnhancedInputSystem)
		{
			Settings->SetInputDeadZoneEnhanced(ActionEnhanced, NewValue);
		}
		else
		{
			Settings->SetInputDeadZoneTraditional(ActionTraditional, NewValue);
		}
	}
}
