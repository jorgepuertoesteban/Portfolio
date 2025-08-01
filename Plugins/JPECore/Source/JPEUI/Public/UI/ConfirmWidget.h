// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ConfirmWidget.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFinishConfirmWidget, bool, bIsSuccessful);
DECLARE_DELEGATE_OneParam(FOnFinishConfirmWidget, bool);

class UButton;
class UTextBlock;


/**
 * 
 */
UCLASS(Abstract)
class JPEUI_API UConfirmWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly)
	TArray<FKey> KeysToCloseDialog;

	UPROPERTY(meta = (BindWidget))
	UButton* AcceptButton;

	UPROPERTY(meta = (BindWidget))
	UButton* CancelButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* BodyText;
public:

	FOnFinishConfirmWidget OnFinishConfirmWidget;

protected:

	virtual bool Initialize() override;

	virtual void NativeConstruct() override;

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

public:

	UFUNCTION(BlueprintCallable)
	void SetBodyText(const FText& NewText);
	
	UFUNCTION()
	void OnAcceptButtonClicked();

	UFUNCTION()
	void OnCancelButtonClicked();
};
