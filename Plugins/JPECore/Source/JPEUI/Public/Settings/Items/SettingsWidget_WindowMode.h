// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Common/SettingsCarouselWidget.h"
#include "GenericPlatform/GenericWindow.h"
#include "SettingsWidget_WindowMode.generated.h"

UCLASS(Abstract)
class JPEUI_API USettingsWidget_WindowMode : public USettingsCarouselWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<TEnumAsByte<EWindowMode::Type>, FText> WindowModeTextsByType;

	UPROPERTY(Transient, BlueprintReadOnly)
	TArray<FString> WindowModes;

protected:

	virtual bool Initialize() override;

public:

	FString WindowModeToString(const EWindowMode::Type WindowMode) const;

protected:

	virtual void GetCarouselOptions(TArray<FString>& OutOptions) override;
	virtual FString GetCurrentSettingValue() const override;
	virtual void UpdateSetting(const int32 NewValue) const override;

};
