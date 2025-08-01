// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Common/SettingsCarouselWidget.h"
#include "SettingsWidget_FrameRate.generated.h"

UCLASS(Abstract)
class JPEUI_API USettingsWidget_FrameRate : public USettingsCarouselWidget
{
	GENERATED_BODY()

protected:

	/** List with all supported frame rates. 0 is unlimited. */
	UPROPERTY(EditAnywhere)
	TArray<int32> SupportedFrameRates;
	UPROPERTY(VisibleAnywhere)
	TMap<float, FString> FrameRates;

public:

	static FString FrameRateToString(int32 FrameRate);

protected:

	virtual bool Initialize() override;

	virtual void GetCarouselOptions(TArray<FString>& OutOptions) override;
	virtual FString GetCurrentSettingValue() const override;
	virtual void UpdateSetting(const int32 NewValue) const override;

};
