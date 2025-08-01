// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Settings/SettingsItemWidget.h"
#include "SettingsSliderWidget.generated.h"

class ULabeledSlider;
class USlider;
class UTextBlock;

UCLASS(Abstract)
class JPEUI_API USettingsSliderWidget : public USettingsItemWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(meta=(BindWidget))
	ULabeledSlider* Slider;

protected:

	virtual void UpdateUIValues() override;

	UFUNCTION()
	void HandleOnSliderValueChanged(const float NewValue);

	virtual FText GetHintText();

	virtual FVector GetCurrentSettingValue() const { return FVector(0.f); }
	virtual void UpdateSetting(const float NewValue) const {}

};
