// Created by Jorge Puerto. All Rights Reserved.


#include "Settings/Items/Common/SettingsCarouselWidget.h"

#include "Common/CarouselSelector.h"

void USettingsCarouselWidget::UpdateUIValues()
{
	if (CarouselSelector)
	{
		CarouselSelector->OnSelectionChanged.Clear();
		CarouselSelector->SelectOption(GetCurrentSettingValue());
		CarouselSelector->OnSelectionChanged.AddDynamic(this, &USettingsCarouselWidget::HandleOnCarouselSelectionChanged);
	}
}

bool USettingsCarouselWidget::Refresh()
{
	if (!Super::Refresh()) return false;
	if (!CarouselSelector) return false;

	GetCarouselOptions(Options);
	CarouselSelector->SetOptions(Options);
		
	return true;
}

void USettingsCarouselWidget::HandleOnCarouselSelectionChanged(const int32 NewOptionIndex)
{
	UpdateSetting(NewOptionIndex);
}