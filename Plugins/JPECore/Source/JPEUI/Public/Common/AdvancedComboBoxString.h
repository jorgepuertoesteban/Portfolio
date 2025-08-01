// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AdvancedComboBoxString.generated.h"

class UComboBoxString;
class UImage;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAdvancedComboBoxStringSelectionChanged, FString, SelectedItem, ESelectInfo::Type, SelectionType);

UCLASS(Abstract)
class JPEUI_API UAdvancedComboBoxString : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnAdvancedComboBoxStringSelectionChanged OnSelectionChanged;

protected:

	UPROPERTY(meta=(BindWidget))
	UComboBoxString* ComboBox;
	UPROPERTY(meta=(BindWidget))
	UImage* CustomOptionArrow;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* CustomOptionText;

public:
	
	UFUNCTION(BlueprintCallable, Category="AdvancedComboBox")
	void AddOption(const FString& Option);
	
	UFUNCTION(BlueprintCallable, Category="AdvancedComboBox")
	void ClearOptions();

	UFUNCTION(BlueprintCallable, Category="AdvancedComboBox")
	void SetSelectedOption(const FString& Option);

	UFUNCTION(BlueprintCallable, Category = "AdvancedComboBox")
	void SetSelectedIndex(const int32 Index);

	UFUNCTION(BlueprintCallable, Category="AdvancedComboBox")
	int32 GetSelectedIndex() const;

protected:

	virtual void NativeConstruct() override;
	
	UFUNCTION()
	virtual void HandleOnComboBoxSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
	
};
