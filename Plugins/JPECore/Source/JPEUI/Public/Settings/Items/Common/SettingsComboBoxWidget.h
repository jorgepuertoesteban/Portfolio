// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Settings/SettingsItemWidget.h"
#include "SettingsComboBoxWidget.generated.h"

class UAdvancedComboBoxString;
class UComboBoxString;

UCLASS(Abstract)
class JPEUI_API USettingsComboBoxWidget : public USettingsItemWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FString> Options;

	UPROPERTY(meta = (BindWidget))
	UAdvancedComboBoxString* ComboBox;

public:

	virtual void UpdateUIValues() override;

	virtual bool Refresh() override;

	UFUNCTION()
	void HandleOnComboBoxSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	virtual void GetComboBoxOptions(TArray<FString>& OutOptions) {}
	virtual FString GetCurrentSettingValue() const { return ""; }
	virtual void UpdateSetting(const int32 NewValue) const {}

};
