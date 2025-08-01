// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Settings/Items/SettingsWidget_Quality.h"
#include "SettingsWidget_ShadingQuality.generated.h"

UCLASS(Abstract)
class JPEUI_API USettingsWidget_ShadingQuality : public USettingsWidget_Quality
{
	GENERATED_BODY()

protected:

	virtual FString GetCurrentSettingValue() const override;
	virtual void UpdateSetting(const int32 NewValue) const override;

};
