// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RevertAppliedSettingsAlertWidget.generated.h"

class UButton;
class UTextBlock;

/**
 * 
 */
UCLASS(Abstract)
class JPEUI_API URevertAppliedSettingsAlertWidget : public UUserWidget
{
	GENERATED_BODY()


protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CountdownDurationSeconds = 11.f;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* TimeRemainingText;
	UPROPERTY(meta=(BindWidget))
	UButton* RevertButton;
	UPROPERTY(meta=(BindWidget))
	UButton* KeepButton;
	
	float CountdownEndTimeSeconds;
	FTimerHandle CountdownTimerHandle;

protected:

	virtual void NativeConstruct() override;

	void RevertAppliedSettings();
	void KeepAppliedSettings();

	UFUNCTION()
	void CountdownTick();
	UFUNCTION()
	void HandleOnRevertButtonClicked();
	UFUNCTION()
	void HandleOnKeepButtonClicked();
	
};
