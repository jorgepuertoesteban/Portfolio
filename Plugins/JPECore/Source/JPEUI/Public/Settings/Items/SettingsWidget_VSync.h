// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Common/SettingsToggleWidget.h"
#include "SettingsWidget_VSync.generated.h"

UCLASS(Abstract)
class JPEUI_API USettingsWidget_VSync : public USettingsToggleWidget
{
	GENERATED_BODY()

protected:

	virtual bool GetCurrentSettingValue() const override;
	virtual void UpdateSetting(const bool bNewValue) const override;

};
