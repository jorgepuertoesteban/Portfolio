// Created by Jorge Puerto. All Rights Reserved.


#include "Settings/Items/SettingsWidget_InputSensitivity.h"

#include "Components/TextBlock.h"
#include "GameFramework/JPEGameUserSettings.h"

bool USettingsWidget_InputSensitivity::Initialize()
{
	if (Super::Initialize())
	{
		if (!NameLabel) return false;

		if (bUseEnhancedInputSystem)
		{
			NameLabel->SetText(FText::FromString(UJPEInputUtils::GetDisplayNameForActionEnhanced(ActionEnhanced).ToString() +  + " Sensitivity"));
		}
		else
		{
			NameLabel->SetText(FText::FromString(UJPEInputUtils::GetDisplayNameForActionTraditional(ActionTraditional).ToString() +  + " Sensitivity"));
		}
		
		return true;
	}
	return false;
}

FVector USettingsWidget_InputSensitivity::GetCurrentSettingValue() const
{
	FVector Result = FVector(MinValue, MaxValue, MinValue);
	if (UJPEGameUserSettings* Settings = UJPEGameUserSettings::GetJPEGameUserSettings())
	{
		if (bUseEnhancedInputSystem)
		{
			Result.Z = Settings->GetInputSensitivityEnhanced(ActionEnhanced);
		}
		else
		{
			Result.Z = Settings->GetInputSensitivityTraditional(ActionTraditional);
		}
	}
	return Result;
}

void USettingsWidget_InputSensitivity::UpdateSetting(const float NewValue) const
{
	if (UJPEGameUserSettings* Settings = UJPEGameUserSettings::GetJPEGameUserSettings())
	{
		if (bUseEnhancedInputSystem)
		{
			Settings->SetInputSensitivityEnhanced(ActionEnhanced, NewValue);
		}
		else
		{
			Settings->SetInputSensitivityTraditional(ActionTraditional, NewValue);
		}
	}
}