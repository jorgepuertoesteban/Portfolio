// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Settings/SettingsItemWidget.h"
#include "SettingsCarouselWidget.generated.h"

class UCarouselSelector;

UCLASS(Abstract)
class JPEUI_API USettingsCarouselWidget : public USettingsItemWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FString> Options;

	UPROPERTY(meta = (BindWidget))
	UCarouselSelector* CarouselSelector;

public:

	virtual void UpdateUIValues() override;

	virtual bool Refresh() override;

	UFUNCTION()
	void HandleOnCarouselSelectionChanged(const int32 NewOptionIndex);

	virtual void GetCarouselOptions(TArray<FString>& OutOptions) {}
	virtual FString GetCurrentSettingValue() const { return ""; }
	virtual void UpdateSetting(const int32 NewValue) const {}

};
