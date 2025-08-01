// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Settings/SettingsItemWidget.h"
#include "Settings/Alerts/DuplicatedKeyAlertWidget.h"
#include "Utils/JPEInputUtils.h"
#include "SettingsInputKeySelectorWidget.generated.h"

class UAdvancedInputKeySelector;

USTRUCT()
struct FDualInputChord
{
	GENERATED_BODY()

	FInputChord Main;
	FInputChord Alt;

	FDualInputChord()
	{
		Main = EKeys::Invalid;
		Alt = EKeys::Invalid;
	}

	FDualInputChord(const FInputChord& InMain, const FInputChord& InAlt)
	{
		Main = InMain;
		Alt = InAlt;
	}
};

UCLASS(Abstract)
class JPEUI_API USettingsInputKeySelectorWidget : public USettingsItemWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UDuplicatedKeyAlertWidget> DuplicatedKeyAlertWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bUseEnhancedInputSystem;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="bUseEnhancedInputSystem", EditConditionHides))
	FMappableActionEnhanced ActionEnhanced;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="bUseEnhancedInputSystem", EditConditionHides))
	FMappableActionEnhanced ActionEnhancedAlt;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="!bUseEnhancedInputSystem", EditConditionHides))
	FMappableActionTraditional ActionTraditional;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="!bUseEnhancedInputSystem", EditConditionHides))
	FMappableActionTraditional ActionTraditionalAlt;

	UPROPERTY(meta=(BindWidget))
	UAdvancedInputKeySelector* InputKeySelector;
	UPROPERTY(meta=(BindWidget))
	UAdvancedInputKeySelector* InputKeySelectorAlt;

protected:

	virtual bool Initialize() override;

	virtual void UpdateUIValues() override;

	UFUNCTION()
	virtual void HandleOnInputKeySelected(const FInputChord& NewValue);
	UFUNCTION()
	virtual void HandleOnAltInputKeySelected(const FInputChord& NewValue);
	UFUNCTION()
	virtual void HandleOnIsSelectingKeyChanged(bool bNewIsSelectingKey);
	UFUNCTION()
	virtual void HandleOnAltIsSelectingKeyChanged(bool bNewIsSelectingKey);

	virtual FDualInputChord GetCurrentSettingValue() const;
	virtual void SaveInputToSettings(const FInputChord& NewValue, const bool bIsMain) const;

	void HandleNewKeyBinding(const FInputChord& NewValue, const bool bIsMain);
	void ShowDuplicatedKeyAlert(const FText& KeyName, const FText& ActionName, const FOnDuplicatedKeyAlertOptionSelected& OnKeepButtonClicked, const FOnDuplicatedKeyAlertOptionSelected& OnRevertButtonClicked);

};
