// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Common/SettingsComboBoxWidget.h"
#include "SettingsWidget_ScreenResolution.generated.h"

UCLASS(Abstract)
class JPEUI_API USettingsWidget_ScreenResolution : public USettingsComboBoxWidget
{
	GENERATED_BODY()

protected:

	/** Map with all supported resolutions. Key is the resolution represented as a raw integer vector and the is the string representing visually the resolution. For example: [Key:(1920,1080), Value: 1920 x 1080 (16:9)] */
	UPROPERTY(EditAnywhere)
	TMap<FIntPoint, FString> Resolutions;

public:

	static FString ResolutionToString(const FIntPoint& Resolution);

protected:

	virtual bool Initialize() override;

	virtual void GetComboBoxOptions(TArray<FString>& OutOptions) override;
	virtual FString GetCurrentSettingValue() const override;
	virtual void UpdateSetting(const int32 NewValue) const override;

};
