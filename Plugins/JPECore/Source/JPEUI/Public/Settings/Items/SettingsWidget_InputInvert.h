// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Common/SettingsToggleWidget.h"
#include "Utils/JPEInputUtils.h"
#include "SettingsWidget_InputInvert.generated.h"

UCLASS()
class JPEUI_API USettingsWidget_InputInvert : public USettingsToggleWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bUseEnhancedInputSystem;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="bUseEnhancedInputSystem", EditConditionHides))
	FMappableActionEnhanced ActionEnhanced;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="!bUseEnhancedInputSystem", EditConditionHides))
	FMappableActionTraditional ActionTraditional;

protected:

	virtual bool Initialize() override;

	virtual bool GetCurrentSettingValue() const override;
	virtual void UpdateSetting(const bool bNewValue) const override;

};
