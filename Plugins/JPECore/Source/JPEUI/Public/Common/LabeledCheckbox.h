// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LabeledCheckbox.generated.h"

class UButton;
class UCheckBox;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCheckedStateChanged, const ECheckBoxState, bNewCheckedState);

UCLASS(Abstract)
class JPEUI_API ULabeledCheckbox : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnCheckedStateChanged OnCheckedStateChanged;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText LabelText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* MainButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCheckBox* MainCheckBox;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* LabelTextBlock;

protected:

	virtual bool Initialize() override;

	virtual void NativeConstruct() override;

public:

	UFUNCTION(BlueprintCallable)
	ECheckBoxState GetCheckedState();

protected:

	UFUNCTION()
	virtual void HandleOnMainButtonClicked();

};
