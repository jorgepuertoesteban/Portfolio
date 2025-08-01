// Created by Jorge Puerto. All Rights Reserved.


#include "Settings/Items/Common/SettingsSliderWidget.h"

#include "Common/LabeledSlider.h"

void USettingsSliderWidget::UpdateUIValues()
{
	if (Slider)
	{
		Slider->OnValueChanged.Clear();
		// X is MinValue, Y is MaxValue, Z is Value
		const FVector CurrentValues = GetCurrentSettingValue();
		Slider->SetMinValue(CurrentValues.X);
		Slider->SetMaxValue(CurrentValues.Y);
		Slider->SetValue(CurrentValues.Z);
		Slider->SetLabelText(GetHintText());
		Slider->OnValueChanged.AddUniqueDynamic(this, &USettingsSliderWidget::HandleOnSliderValueChanged);
	}
}

void USettingsSliderWidget::HandleOnSliderValueChanged(const float NewValue)
{
	UpdateSetting(NewValue);
	if (Slider)
	{
		Slider->SetValue(NewValue);
		Slider->SetLabelText(GetHintText());
	}
}

FText USettingsSliderWidget::GetHintText()
{
	if (Slider)
	{
		if (Slider->GetMinValue() == 0.f && Slider->GetMaxValue() == 1.f)
		{
			return FText::FromString(FString::FromInt(FMath::RoundToInt(Slider->GetValue() * 100.f)) + "%");
		}
		return FText::FromString(FString::Printf(TEXT("%.2f"), Slider->GetValue()));
	}
	return FText::FromString("");
}