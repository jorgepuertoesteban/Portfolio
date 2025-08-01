// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LabeledSlider.generated.h"

class USlider;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLabeledSliderValueChanged, const float, NewValue);

UCLASS(Abstract)
class JPEUI_API ULabeledSlider : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnLabeledSliderValueChanged OnValueChanged;

protected:
	
	UPROPERTY(meta=(BindWidget))
	USlider* Slider;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* SliderHint;

public:

	float GetValue() const;
	float GetMinValue() const;
	void SetMinValue(const float NewMinValue) const;
	float GetMaxValue() const;
	void SetMaxValue(const float NewMaxValue) const;
	void SetValue(const float NewValue) const;
	void SetLabelText(const FText& NewLabelText) const;
	
protected:

	virtual void NativeConstruct() override;

	UFUNCTION()
	void HandleOnSliderValueChanged(const float NewValue);
	
};
