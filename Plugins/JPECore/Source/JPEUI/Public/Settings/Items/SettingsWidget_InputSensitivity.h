// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Common/SettingsSliderWidget.h"
#include "Utils/JPEInputUtils.h"
#include "SettingsWidget_InputSensitivity.generated.h"

UCLASS()
class JPEUI_API USettingsWidget_InputSensitivity : public USettingsSliderWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bUseEnhancedInputSystem;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="bUseEnhancedInputSystem", EditConditionHides))
	FMappableActionEnhanced ActionEnhanced;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="!bUseEnhancedInputSystem", EditConditionHides))
	FMappableActionTraditional ActionTraditional;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MinValue = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxValue = 1.f;

protected:

	virtual bool Initialize() override;

	virtual FVector GetCurrentSettingValue() const override;
	virtual void UpdateSetting(const float NewValue) const override;

};
