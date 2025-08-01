// Created by Jorge Puerto. All Rights Reserved.


#include "Common/LabeledSlider.h"

#include "Components/Slider.h"
#include "Components/TextBlock.h"

float ULabeledSlider::GetValue() const
{
	return Slider ? Slider->GetValue() : 0.f;
}

float ULabeledSlider::GetMinValue() const
{
	if (Slider)
	{
		return Slider->GetMinValue();
	}
	return 0.f;
}

void ULabeledSlider::SetMinValue(const float NewMinValue) const
{
	if (Slider)
	{
		Slider->SetMinValue(NewMinValue);
	}
}

float ULabeledSlider::GetMaxValue() const
{
	if (Slider)
	{
		return Slider->GetMaxValue();
	}
	return 1.f;
}

void ULabeledSlider::SetMaxValue(const float NewMaxValue) const
{
	if (Slider)
	{
		Slider->SetMaxValue(NewMaxValue);
	}
}

void ULabeledSlider::SetValue(const float NewValue) const
{
	if (Slider)
	{
		Slider->SetValue(NewValue);
	}
}

void ULabeledSlider::SetLabelText(const FText& NewLabelText) const
{
	if (SliderHint)
	{
		SliderHint->SetText(NewLabelText);
	}
}

void ULabeledSlider::NativeConstruct()
{
	Super::NativeConstruct();

	if (Slider)
	{
		Slider->OnValueChanged.AddUniqueDynamic(this, &ULabeledSlider::HandleOnSliderValueChanged);
	}
}

void ULabeledSlider::HandleOnSliderValueChanged(const float NewValue)
{
	OnValueChanged.Broadcast(NewValue);
}