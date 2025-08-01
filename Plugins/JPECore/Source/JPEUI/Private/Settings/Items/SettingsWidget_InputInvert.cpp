// Created by Jorge Puerto. All Rights Reserved.


#include "Settings/Items/SettingsWidget_InputInvert.h"

#include "Components/TextBlock.h"
#include "GameFramework/JPEGameUserSettings.h"

bool USettingsWidget_InputInvert::Initialize()
{
	if (Super::Initialize())
	{
		if (!NameLabel) return false;

		if (bUseEnhancedInputSystem)
		{
			NameLabel->SetText(FText::FromString("Invert " + UJPEInputUtils::GetDisplayNameForActionEnhanced(ActionEnhanced).ToString()));
		}
		else
		{
			NameLabel->SetText(FText::FromString("Invert " + UJPEInputUtils::GetDisplayNameForActionTraditional(ActionTraditional).ToString()));
		}
		
		return true;
	}
	return false;
}

bool USettingsWidget_InputInvert::GetCurrentSettingValue() const
{
	if (UJPEGameUserSettings* Settings = UJPEGameUserSettings::GetJPEGameUserSettings())
	{
		if (bUseEnhancedInputSystem)
		{
			return Settings->GetInputInvertEnhanced(ActionEnhanced);
		}
		return Settings->GetInputInvertTraditional(ActionTraditional);
	}
	return false;
}

void USettingsWidget_InputInvert::UpdateSetting(const bool bNewValue) const
{
	if (UJPEGameUserSettings* Settings = UJPEGameUserSettings::GetJPEGameUserSettings())
	{
		if (bUseEnhancedInputSystem)
		{
			Settings->SetInputInvertEnhanced(ActionEnhanced, bNewValue);
		}
		else
		{
			Settings->SetInputInvertTraditional(ActionTraditional, bNewValue);
		}
	}
}