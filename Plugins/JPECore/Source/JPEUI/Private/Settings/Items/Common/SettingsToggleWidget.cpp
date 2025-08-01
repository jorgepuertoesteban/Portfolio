// Created by Jorge Puerto. All Rights Reserved.


#include "Settings/Items/Common/SettingsToggleWidget.h"

#include "Components/PanelWidget.h"
#include "Common/ToggleButton.h"

void USettingsToggleWidget::UpdateUIValues()
{
	if (ToggleButton)
	{
		ToggleButton->OnValueChanged.Clear();
		ToggleButton->SetValue(GetCurrentSettingValue());
		ToggleButton->OnValueChanged.AddUniqueDynamic(this, &USettingsToggleWidget::HandleOnToggleValueChanged);
	}
}

void USettingsToggleWidget::HandleOnToggleValueChanged(const bool NewValue)
{
	UpdateSetting(NewValue);
}