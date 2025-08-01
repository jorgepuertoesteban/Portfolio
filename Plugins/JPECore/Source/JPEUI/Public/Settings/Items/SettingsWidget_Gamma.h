// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Common/SettingsSliderWidget.h"
#include "SettingsWidget_Gamma.generated.h"

UCLASS(Abstract)
class JPEUI_API USettingsWidget_Gamma : public USettingsSliderWidget
{
	GENERATED_BODY()

protected:

	virtual FVector GetCurrentSettingValue() const override;
	virtual void UpdateSetting(const float NewValue) const override;

};
