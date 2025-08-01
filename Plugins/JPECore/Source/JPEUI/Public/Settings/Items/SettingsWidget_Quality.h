// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Common/SettingsCarouselWidget.h"
#include "SettingsWidget_Quality.generated.h"

UCLASS(Abstract)
class JPEUI_API USettingsWidget_Quality : public USettingsCarouselWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ClampMin = 1, ClampMax = 5))
	int32 NumAvailableQualities = 4;

	UPROPERTY(Transient, BlueprintReadOnly)
	TArray<FString> Qualities;

protected:

	virtual bool Initialize() override;
	virtual void NativeConstruct() override;

	virtual FString QualityLevelToString(const int32 QualityLevel) const;

	virtual void GetCarouselOptions(TArray<FString>& OutOptions) override;

	UFUNCTION()
	void HandleOnQualitySettingsEdited();

};
