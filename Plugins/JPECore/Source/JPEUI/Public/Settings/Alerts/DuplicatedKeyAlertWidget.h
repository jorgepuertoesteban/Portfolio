// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DuplicatedKeyAlertWidget.generated.h"

class UButton;
class UTextBlock;

DECLARE_DELEGATE(FOnDuplicatedKeyAlertOptionSelected)

/**
 * 
 */
UCLASS(Abstract)
class JPEUI_API UDuplicatedKeyAlertWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	FOnDuplicatedKeyAlertOptionSelected OnRevertButtonClicked;
	FOnDuplicatedKeyAlertOptionSelected OnKeepButtonClicked;
	
protected:
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* KeyName;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ActionName;
	UPROPERTY(meta=(BindWidget))
	UButton* RevertButton;
	UPROPERTY(meta=(BindWidget))
	UButton* KeepButton;

public:

	UFUNCTION(BlueprintCallable)
	void SetKeyName(const FText& NewKeyName) const;
	UFUNCTION(BlueprintCallable)
	void SetActionName(const FText& NewActionName) const;
	
protected:
	
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void HandleOnRevertButtonClicked();
	UFUNCTION()
	void HandleOnKeepButtonClicked();
	
};
