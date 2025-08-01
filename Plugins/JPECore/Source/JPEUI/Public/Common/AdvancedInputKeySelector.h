// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AdvancedInputKeySelector.generated.h"

class UBorder;
class UImage;
class UInputKeySelector;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnKeySelected, const FInputChord&, SelectedKey);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIsSelectingKeyChanged, const bool, bNewIsSelectingKey);

UCLASS(Abstract)
class JPEUI_API UAdvancedInputKeySelector : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnKeySelected OnKeySelected;
	UPROPERTY(BlueprintAssignable)
	FOnIsSelectingKeyChanged OnIsSelectingKeyChanged;
	
protected:
	
	UPROPERTY(EditAnywhere)
	TMap<FKey, UTexture2D*> InputKeyTextures;
	
	UPROPERTY(meta=(BindWidget))
	UInputKeySelector* InputKeySelector;
	UPROPERTY(meta=(BindWidget))
	UBorder* InputKeyBorder;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* InputKeyText;
	UPROPERTY(meta=(BindWidget))
	UImage* InputKeyImage;

public:

	void SetSelectedKey(const FInputChord& NewKey) const;
	void SetText(const FText& NewText) const;
	void SetTexture(UTexture2D* NewTexture) const;
	
	void UpdateVisuals() const;

protected:

	virtual bool Initialize() override;
	virtual void NativeConstruct() override;

	UFUNCTION()
	void HandleOnInputKeySelected(const FInputChord SelectedKey);
	UFUNCTION()
	void HandleOnOnIsSelectingKeyChanged();
	
};
