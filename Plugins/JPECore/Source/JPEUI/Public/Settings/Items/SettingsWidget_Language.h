// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Common/SettingsComboBoxWidget.h"
#include "SettingsWidget_Language.generated.h"

UCLASS(Abstract)
class JPEUI_API USettingsWidget_Language : public USettingsComboBoxWidget
{
	GENERATED_BODY()

protected:

	/** Map with all supported languages. Key must be the IETF tag of the language and the value must be the string representing visually the language. For example: [Key:en-US, Value: English (United States)] */
	UPROPERTY(EditAnywhere)
	TMap<FString, FString> Languages;

protected:

	virtual void GetComboBoxOptions(TArray<FString>& OutOptions) override;
	virtual FString GetCurrentSettingValue() const override;
	virtual void UpdateSetting(const int32 NewValue) const override;

};
