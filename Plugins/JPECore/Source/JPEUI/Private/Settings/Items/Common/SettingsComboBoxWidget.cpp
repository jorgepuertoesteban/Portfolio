// Created by Jorge Puerto. All Rights Reserved.


#include "Settings/Items/Common/SettingsComboBoxWidget.h"

#include "Common/AdvancedComboBoxString.h"

void USettingsComboBoxWidget::UpdateUIValues()
{
	if (ComboBox)
	{
		ComboBox->OnSelectionChanged.Clear();
		ComboBox->SetSelectedOption(GetCurrentSettingValue());
		ComboBox->OnSelectionChanged.AddDynamic(this, &USettingsComboBoxWidget::HandleOnComboBoxSelectionChanged);
	}
}

bool USettingsComboBoxWidget::Refresh()
{
	if (!Super::Refresh()) return false;
	if (!ComboBox) return false;

	GetComboBoxOptions(Options);
	ComboBox->ClearOptions();
	for (FString& Option : Options)
	{
		ComboBox->AddOption(Option);
	}
	ComboBox->SetSelectedIndex(0);
		
	return true;
}

void USettingsComboBoxWidget::HandleOnComboBoxSelectionChanged(FString, ESelectInfo::Type)
{
	UpdateSetting(ComboBox->GetSelectedIndex());
}