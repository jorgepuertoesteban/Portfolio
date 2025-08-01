// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Settings/SettingsItemWidget.h"
#include "SettingsToggleWidget.generated.h"

class UToggleButton;

UCLASS(Abstract)
class JPEUI_API USettingsToggleWidget : public USettingsItemWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(meta=(BindWidget))
	UToggleButton* ToggleButton;

protected:

	virtual void UpdateUIValues() override;

	UFUNCTION()
	void HandleOnToggleValueChanged(const bool bNewValue);

	virtual bool GetCurrentSettingValue() const { return false; }
	virtual void UpdateSetting(const bool bNewValue) const {}
};
